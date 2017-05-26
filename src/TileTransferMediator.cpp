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
    
    int maxTileThrowDist = fromTileStack.tile().model().maxThrowDistance();
    if (world.tileManhattanDistance(from.pos, to.pos) > maxTileThrowDist) return;

    int toX = to.pos.x;
    int toY = to.pos.y;
    TileColumn& toTileColumn = map.at(toX, toY);
    TileStack& toTileStack = toTileColumn.top();
    if (!toTileStack.tile().model().isMovableTo()) return;

    if (!world.lineOfSightBetweenPlayerAndTile(from.pos)) return;

    // check line of sight between tiles
    if (!world.lineOfSightBetweenTiles(from.pos, to.pos)) return;

    // perform move
    std::cout << "World -> World\n";

    toTileColumn.placeOnTop(fromTileColumn.takeFromTop());

    EventDispatcher::instance().broadcast<TileMovedFromWorldToWorld>(TileMovedFromWorldToWorld{ from, to, &(toTileColumn.top()) });
}
void TileTransferMediator::operator()(const FromWorld& from, const ToInventory& to)
{
    World& world = *from.world;
    MapLayer& map = world.map();

    int fromX = from.pos.x;
    int fromY = from.pos.y;
    TileColumn& fromTileColumn = map.at(fromX, fromY);
    TileStack& fromTileStack = fromTileColumn.top();
    if (!fromTileStack.tile().model().isMovableFrom()) return; // item can't be moved

    Inventory& inventory = *to.inventory;
    const int slot = to.slot;
    if (!inventory.at(slot).isEmpty()) return; // no space to place the tile

    InventorySystem& inventorySystem = *to.inventorySystem;
    if (!inventorySystem.canStore(inventory, fromTileStack.tile())) return;

    // perform move
    std::cout << "World -> Inventory\n";

    TileStack detachedTileStack = fromTileColumn.takeFromTop();
    inventory.at(slot) = std::move(detachedTileStack);

    EventDispatcher::instance().broadcast<TileMovedFromWorldToInventory>(TileMovedFromWorldToInventory{ from, to, &(inventory.at(slot)) });
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

    // perform move
    std::cout << "Inventory -> World\n";

    toTileColumn.placeOnTop(std::move(fromTileStack));

    EventDispatcher::instance().broadcast<TileMovedFromInventoryToWorld>(TileMovedFromInventoryToWorld{ from, to, &(toTileColumn.top()) });
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
    if (!toInventory.at(toSlot).isEmpty()) return; // no space to place the tile

    if (!inventorySystem.canStore(toInventory, fromTileStack.tile())) return;

    // perform move
    std::cout << "Inventory -> Inventory\n";

    toInventory.at(toSlot) = std::move(fromTileStack);

    EventDispatcher::instance().broadcast<TileMovedFromInventoryToInventory>(TileMovedFromInventoryToInventory{ from, to, &(toInventory.at(toSlot)) });
}