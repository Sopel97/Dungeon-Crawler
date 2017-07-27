#include "Inventory.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/TilePrefab.h"
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
void Inventory::placeTile(TileStack&& tileStack, int slot)
{
    TileStack& stack = at(slot);
    if (!stack.isEmpty()) takeTile(slot);

    stack = std::move(tileStack);
    onTilePlaced(stack, slot);
}
void Inventory::insertMerge(TileStack&& tileStack)
{
    const int numSlots = size();
    for (int slot = 0; slot < numSlots && !tileStack.isEmpty(); ++slot)
    {
        TileStack& tileStackInSlot = at(slot);
        if (tileStackInSlot.isEmpty() && meetsRequirements(tileStack.tile(), slot))
        {
            placeTile(std::move(tileStack), slot);
            return;
        }

        if (tileStackInSlot.tile().equals(tileStack.tile()))
        {
            const int spaceLeft = tileStackInSlot.maxQuantity() - tileStackInSlot.quantity();
            const int toMove = std::min(spaceLeft, tileStack.quantity());
            if (toMove > 0)
            {
                tileStackInSlot.addTiles(toMove);
                tileStack.removeTiles(toMove);
            }
        }
    }
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
    at(slot).addTiles(count);
}
void Inventory::removeTiles(int slot, int count)
{
    TileStack& stack = at(slot);
    if (stack.isEmpty()) return;

    if (stack.quantity() <= count) onTileRemoved(stack, slot);

    stack.removeTiles(count);
}
TileStack Inventory::splitTiles(int slot, int count)
{
    TileStack& stack = at(slot);
    if (stack.isEmpty()) return TileStack();

    if (stack.quantity() <= count) onTileRemoved(stack, slot);

    return stack.split(count);
}
void Inventory::useTile(Player& player, int slot)
{
    TileStack& tileStack = at(slot);
    if (tileStack.isEmpty()) return;

    auto result = tileStack.tile().model().use(player, InventorySlotLocation(*this, slot));

    if (result.numUsed > 0)
    {
        removeTiles(slot, result.numUsed);
    }

    for (const auto& creationRequest : result.tileCreationRequests)
    {
        TileStack tileStackToPlace(creationRequest.prefab.get().instantiate(), creationRequest.numToCreate);
        insertMerge(std::move(tileStackToPlace));

        if (!tileStackToPlace.isEmpty())
        {
            // TODO: fallback to World
        }
    }
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