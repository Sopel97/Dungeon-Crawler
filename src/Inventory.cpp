#include "Inventory.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/models/TileModel.h"

#include "EventDispatcher.h"

#include "events/TilePlacedInInventory.h"
#include "events/TileRemovedFromInventory.h"

#include <algorithm>

Inventory::Inventory()
{

}
Inventory::~Inventory()
{

}
TileStack& Inventory::at(size_t slotId)
{
    return this->contents()[slotId];
}
const TileStack& Inventory::at(size_t slotId) const
{
    return this->contents()[slotId];
}
bool Inventory::meetsRequirements(const Tile& tile, int slot) const
{
    return tile.model().meetsRequirements(this->slotContentRequirement(slot));
}
void Inventory::placeTile(TileStack&& tile, int slot)
{
    TileStack& stack = at(slot);
    if (!stack.isEmpty()) takeTile(slot);

    stack = std::move(tile);
    onTilePlaced(stack, slot);
}
TileStack Inventory::takeTile(int slot)
{
    onTileRemoved(at(slot), slot);

    TileStack stack = std::move(at(slot));
    at(slot) = TileStack();

    return stack;
}

void Inventory::addTiles(int slot, int count)
{
    at(slot).insert(count);
}
void Inventory::removeTiles(int slot, int count)
{
    TileStack& stack = at(slot);

    if (stack.quantity() <= count) onTileRemoved(stack, slot);

    stack.erase(count);
}
void Inventory::onTilePlaced(TileStack& stack, int slot)
{
    stack.tile().onTilePlaced(*this, slot);
    EventDispatcher::instance().broadcast<TilePlacedInInventory>(TilePlacedInInventory{ &stack, this, slot });
}
void Inventory::onTileRemoved(TileStack& stack, int slot)
{
    stack.tile().onTileRemoved(*this, slot);
    EventDispatcher::instance().broadcast<TileRemovedFromInventory>(TileRemovedFromInventory{ &stack, this, slot });
}