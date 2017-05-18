#include "TileTransferMediator.h"

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

void TileTransferMediator::grabFromWorld(const ls::Vec2I& loc, World& world, Player& player)
{
    m_source.emplace(FromWorld{ loc, &world, &player });
}
void TileTransferMediator::grabFromInventory(InventorySystem& invSys, Inventory& inv, int slot)
{
    m_source.emplace(FromInventory{ &invSys, &inv, slot });
}
void TileTransferMediator::putToWorld(const ls::Vec2I& loc)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(ToWorld{ loc });
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}
void TileTransferMediator::putToInventory(Inventory& inv, int slot)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(ToInventory{ &inv, slot });
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}
void TileTransferMediator::reset()
{
    std::cout << "Reset\n";
    m_source.reset();
}

bool TileTransferMediator::isAnyTileGrabbed() const
{
    return m_source.has_value();
}
void TileTransferMediator::operator()(const FromWorld& from, const ToWorld& to)
{
    // check validity

    if (from.pos.x == to.pos.x && from.pos.y == to.pos.y) return;

    MapLayer& map = from.world->map();

    int fromX = from.pos.x;
    int fromY = from.pos.y;
    TileColumn& fromTileColumn = map.at(fromX, fromY);
    TileStack& tromTileStack = fromTileColumn.top();
    if (!tromTileStack.tile().model().isMovableFrom()) return;

    int toX = to.pos.x;
    int toY = to.pos.y;
    TileColumn& toTileColumn = map.at(toX, toY);
    TileStack& toTileStack = toTileColumn.top();
    if (!toTileStack.tile().model().isMovableTo()) return;

    // perform move
    std::cout << "World -> World\n";

    TileStack* fromTileStackDetached = fromTileColumn.releaseTop();
    toTileColumn.push(fromTileStackDetached);

    //TODO: make this a global event
    from.world->onTileMovedFromWorldToWorld(TileMovedFromWorldToWorld{from, to, fromTileStackDetached});
}
void TileTransferMediator::operator()(const FromWorld& from, const ToInventory& to)
{
    std::cout << "World -> Inventory\n";
}
void TileTransferMediator::operator()(const FromInventory& from, const ToWorld& to)
{
    std::cout << "Inventory -> World\n";
}
void TileTransferMediator::operator()(const FromInventory& from, const ToInventory& to)
{
    std::cout << "Inventory -> Inventory\n";
}