#include "Inventory.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/models/TileModel.h"

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