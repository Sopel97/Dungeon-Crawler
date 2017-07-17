#include "TileTransferMediator.h"

#include "EventDispatcher.h"
#include "Player.h"
#include "World.h"
#include "Inventory.h"
#include "InventorySystem.h"
#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"
#include "tiles/TileStack.h"
#include "TileColumn.h"
#include "MapLayer.h"
#include "events/TileMovedFromWorldToWorld.h"
#include "events/TileMovedFromWorldToInventory.h"
#include "events/TileMovedFromInventoryToInventory.h"
#include "events/TileMovedFromInventoryToWorld.h"

#include "Logger.h"

void TileTransferMediator::grabFromWorld(const ls::Vec2I& loc, World& world, Player& player)
{
    m_source.emplace(FromWorld{ loc, &world, &player });
}
void TileTransferMediator::grabFromInventory(InventorySystem& invSys, Inventory& inv, int slot)
{
    m_source.emplace(FromInventory{ &invSys, &inv, slot });
}
void TileTransferMediator::putToWorld(const ls::Vec2I& loc, World& world, Player& player)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(ToWorld{ loc, &world, &player });
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}
void TileTransferMediator::putToInventory(InventorySystem& invSys, Inventory& inv, int slot)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(ToInventory{ &invSys, &inv, slot });
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}
void TileTransferMediator::reset()
{
    m_source.reset();
}

bool TileTransferMediator::isAnyTileGrabbed() const
{
    return m_source.has_value();
}
void TileTransferMediator::operator()(const FromWorld& from, const ToWorld& to)
{
    constexpr float maxPlayerDistFromTile = 48.0f;
    // check validity

    if (from.pos.x == to.pos.x && from.pos.y == to.pos.y) return;

    World& world = *from.world;
    MapLayer& map = world.map();

    if (world.playerDistanceToTile(from.pos) > maxPlayerDistFromTile) return;

    int fromX = from.pos.x;
    int fromY = from.pos.y;
    TileColumn& fromTileColumn = map.at(fromX, fromY);
    TileStack& fromTileStack = fromTileColumn.top();
    if (!fromTileStack.tile().model().isMovableFrom()) return;

    int toX = to.pos.x;
    int toY = to.pos.y;
    TileColumn& toTileColumn = map.at(toX, toY);
    TileStack& toTileStack = toTileColumn.top();
    if (!toTileStack.tile().model().isMovableTo()) return;

    int maxTileThrowDist = fromTileStack.tile().model().maxThrowDistance();
    if (world.tileManhattanDistanceFromPlayer(to.pos) > maxTileThrowDist) return; // out of range

    if (!world.lineOfSightBetweenPlayerAndTile(from.pos)) return;

    // check line of sight between tiles
    if (!world.lineOfSightBetweenTiles(from.pos, to.pos)) return;

    // perform move
    Logger::instance().log(Logger::Priority::Debug, "moving tile: World -> World");
    auto stacksToCauseEvent = move(map, fromX, fromY, toX, toY, fromTileStack.quantity());

    for (int tileZ : stacksToCauseEvent)
    {
        EventDispatcher::instance().broadcast<TileMovedFromWorldToWorld>(TileMovedFromWorldToWorld{ from, to, &(toTileColumn.at(tileZ)) });
    }
}
void TileTransferMediator::operator()(const FromWorld& from, const ToInventory& to)
{
    constexpr float maxPlayerDistFromTile = 48.0f;

    World& world = *from.world;
    MapLayer& map = world.map();

    int fromX = from.pos.x;
    int fromY = from.pos.y;
    TileColumn& fromTileColumn = map.at(fromX, fromY);
    TileStack& fromTileStack = fromTileColumn.top();

    if (world.playerDistanceToTile(from.pos) > maxPlayerDistFromTile) return; // out of range
    if (!fromTileStack.tile().model().isMovableFrom()) return; // item can't be moved
    if (!world.lineOfSightBetweenPlayerAndTile(from.pos)) return; // no line of sight

    Inventory& inventory = *to.inventory;
    const int slot = to.slot;
    InventorySystem& inventorySystem = *to.inventorySystem;
    TileStack& toTileStack = inventory.at(slot);

    if (!toTileStack.isEmpty() && toTileStack.tile().model().inventory() != nullptr)
    {
        Inventory& inventoryInside = *toTileStack.tile().model().inventory();
        if (!inventorySystem.canStore(inventoryInside, fromTileStack.tile(), inventory)) return;

        auto slotsToCauseEvent = move(map, fromX, fromY, inventoryInside, fromTileStack.quantity());

        for (int slotId : slotsToCauseEvent)
        {
            EventDispatcher::instance().broadcast<TileMovedFromWorldToInventory>(TileMovedFromWorldToInventory{ from, to, &(inventoryInside.at(slotId)) }); //temp
        }
    }
    else
    {
        if (!inventorySystem.canStore(inventory, fromTileStack.tile())) return;
        if (!inventory.meetsRequirements(fromTileStack.tile(), slot)) return;

        if (move(map, fromX, fromY, inventory, slot, fromTileStack.quantity()))
        {
            EventDispatcher::instance().broadcast<TileMovedFromWorldToInventory>(TileMovedFromWorldToInventory{ from, to, &(inventory.at(slot)) });
        }
    }
    Logger::instance().log(Logger::Priority::Debug, "moving tile: World -> Inventory");
}
void TileTransferMediator::operator()(const FromInventory& from, const ToWorld& to)
{
    InventorySystem& inventorySystem = *from.inventorySystem;
    Inventory& fromInventory = *from.inventory;
    int fromSlot = from.slot;
    TileStack& fromTileStack = fromInventory.at(fromSlot);

    if (fromTileStack.isEmpty()) return; // nothing to move

    int toX = to.pos.x;
    int toY = to.pos.y;
    World& world = *to.world;
    MapLayer& map = world.map();
    TileColumn& toTileColumn = map.at(toX, toY);
    TileStack& toTileStack = toTileColumn.top();

    if (!toTileStack.tile().model().isMovableTo()) return; // cant be moved to this tile

    int maxTileThrowDist = fromTileStack.tile().model().maxThrowDistance();
    if (world.tileManhattanDistanceFromPlayer(to.pos) > maxTileThrowDist) return; // out of range

    if (!world.lineOfSightBetweenPlayerAndTile(to.pos)) return; // out of sight

    // perform move
    Logger::instance().log(Logger::Priority::Debug, "moving tile: Inventory -> World");

    auto stacksToCauseEvent = move(fromInventory, fromSlot, map, toX, toY, fromTileStack.quantity());

    for (int tileZ : stacksToCauseEvent)
    {
        EventDispatcher::instance().broadcast<TileMovedFromInventoryToWorld>(TileMovedFromInventoryToWorld{ from, to, &(toTileColumn.at(tileZ)) });
    }
}
void TileTransferMediator::operator()(const FromInventory& from, const ToInventory& to)
{
    if (from.inventory == to.inventory && from.slot == to.slot) return;

    InventorySystem& inventorySystem = *from.inventorySystem;

    Inventory& fromInventory = *from.inventory;
    const int fromSlot = from.slot;
    TileStack& fromTileStack = fromInventory.at(fromSlot);
    if (fromTileStack.isEmpty()) return; // nothing to move
    if (!fromTileStack.tile().model().isMovableFrom()) return; // item can't be moved

    Inventory& toInventory = *to.inventory;
    const int toSlot = to.slot;
    TileStack& toTileStack = toInventory.at(toSlot);
    if (!toTileStack.isEmpty() && toTileStack.tile().model().inventory() != nullptr)
    {
        Inventory& inventoryInside = *toTileStack.tile().model().inventory();
        if (&inventoryInside == &fromInventory) return;
        if (!inventorySystem.canStore(inventoryInside, fromTileStack.tile(), toInventory)) return;

        auto slotsToCauseEvent = move(fromInventory, fromSlot, inventoryInside, fromTileStack.quantity());

        for (int slotId : slotsToCauseEvent)
        {
            EventDispatcher::instance().broadcast<TileMovedFromInventoryToInventory>(TileMovedFromInventoryToInventory{ from, to, &(inventoryInside.at(slotId)) }); //temp
        }
    }
    else
    {
        if (!inventorySystem.canStore(toInventory, fromTileStack.tile())) return;
        if (!toInventory.meetsRequirements(fromTileStack.tile(), toSlot)) return;

        if (move(fromInventory, fromSlot, toInventory, toSlot, fromTileStack.quantity()))
        {
            EventDispatcher::instance().broadcast<TileMovedFromInventoryToInventory>(TileMovedFromInventoryToInventory{ from, to, &(toInventory.at(toSlot)) });
        }
    }

    // perform move
    Logger::instance().log(Logger::Priority::Debug, "moving tile: Inventory -> Inventory");

}
std::vector<int> TileTransferMediator::move(MapLayer& map, int fromX, int fromY, int toX, int toY, int max)
{
    std::vector<int> stacksToCauseEvent;

    TileColumn& fromColumn = map.at(fromX, fromY);

    const int fromZ = fromColumn.topZ();
    TileStack& from = fromColumn.top();

    TileColumn& toColumn = map.at(toX, toY);

    const int maxQuantity = from.maxQuantity();
    if (maxQuantity != 1)
    {
        TileStack& currentTop = toColumn.top();
        if (from.tile().equals(currentTop.tile()))
        {
            const int toMove = std::min(max, currentTop.maxQuantity() - currentTop.quantity());

            map.addTiles(toX, toY, toColumn.topZ(), toMove);
            map.removeTiles(fromX, fromY, fromZ, toMove);
            max -= toMove;

            stacksToCauseEvent.push_back(toColumn.topZ());
        }
    }

    if (!from.isEmpty() && max > 0)
    {
        const int toMove = std::min(from.quantity(), max);
        map.placeTile(map.splitTiles(fromX, fromY, fromZ, toMove), toX, toY);

        stacksToCauseEvent.push_back(toColumn.topZ());
    }

    return stacksToCauseEvent;
}
int TileTransferMediator::move(MapLayer& map, int fromX, int fromY, Inventory& inventory, int toSlot, int max)
{
    int tilesMoved = 0;

    TileColumn& fromColumn = map.at(fromX, fromY);
    const int fromZ = fromColumn.topZ();
    TileStack& from = fromColumn.top();
    TileStack& to = inventory.at(toSlot);

    if (to.isEmpty())
    {
        const int toMove = std::min(from.quantity(), max);
        inventory.placeTile(map.splitTiles(fromX, fromY, fromZ, toMove), toSlot);

        tilesMoved += toMove;
    }
    else if (to.tile().equals(from.tile()))
    {
        const int toMove = std::min(from.quantity(), to.maxQuantity() - to.quantity());
        if (toMove == 0) return false;
        
        map.removeTiles(fromX, fromY, fromZ, toMove);
        inventory.addTiles(toSlot, toMove);

        tilesMoved += toMove;
    }

    return tilesMoved;
}
std::vector<int> TileTransferMediator::move(MapLayer& map, int fromX, int fromY, Inventory& inventory, int max)
{
    std::vector<int> slotsToCauseEvents;

    TileColumn& fromColumn = map.at(fromX, fromY);
    const int fromZ = fromColumn.topZ();

    const int invSize = inventory.size();
    for (int slotId = 0; slotId < invSize && max > 0; ++slotId)
    {
        const int tilesMoved = move(map, fromX, fromY, inventory, slotId, max);
        if (tilesMoved)
        {
            max -= tilesMoved;

            slotsToCauseEvents.push_back(slotId);
        }
    }

    return slotsToCauseEvents;
}
int TileTransferMediator::move(Inventory& fromInventory, int fromSlot, Inventory& toInventory, int toSlot, int max)
{
    int tilesMoved = 0;

    TileStack& from = fromInventory.at(fromSlot);
    TileStack& to = toInventory.at(toSlot);

    if (to.isEmpty())
    {
        const int toMove = std::min(from.quantity(), max);
        toInventory.placeTile(fromInventory.splitTiles(fromSlot, toMove), toSlot);

        tilesMoved += toMove;
    }
    else if (to.tile().equals(from.tile()))
    {
        const int toMove = std::min(from.quantity(), to.maxQuantity() - to.quantity());
        if (toMove == 0) return false;

        fromInventory.removeTiles(fromSlot, toMove);
        toInventory.addTiles(toSlot, toMove);

        tilesMoved += toMove;
    }

    return tilesMoved;
}
std::vector<int> TileTransferMediator::move(Inventory& fromInventory, int fromSlot, Inventory& toInventory, int max)
{
    std::vector<int> slotsToCauseEvents;

    TileStack& from = fromInventory.at(fromSlot);

    const int invSize = toInventory.size();
    for (int slotId = 0; slotId < invSize && max > 0; ++slotId)
    {
        const int tilesMoved = move(fromInventory, fromSlot, toInventory, slotId, max);
        if (tilesMoved)
        {
            max -= tilesMoved;

            slotsToCauseEvents.push_back(slotId);
        }
    }

    return slotsToCauseEvents;
}
std::vector<int> TileTransferMediator::move(Inventory& fromInventory, int fromSlot, MapLayer& map, int toX, int toY, int max)
{
    std::vector<int> stacksToCauseEvents;

    TileStack& from = fromInventory.at(fromSlot);
    TileColumn& toColumn = map.at(toX, toY);

    const int maxQuantity = from.maxQuantity();
    if (maxQuantity != 1)
    {
        TileStack& currentTop = toColumn.top();
        const int toZ = toColumn.topZ();
        if (from.tile().equals(currentTop.tile()))
        {
            const int toMove = std::min(max, currentTop.maxQuantity() - currentTop.quantity());

            map.addTiles(toX, toY, toZ, toMove);
            fromInventory.removeTiles(fromSlot, toMove);
            max -= toMove;

            stacksToCauseEvents.push_back(toZ);
        }
    }

    if (!from.isEmpty() && max > 0)
    {
        const int toMove = std::min(from.quantity(), max);

        map.placeTile(fromInventory.splitTiles(fromSlot, toMove), toX, toY);

        stacksToCauseEvents.push_back(toColumn.topZ());
    }

    return stacksToCauseEvents;
}