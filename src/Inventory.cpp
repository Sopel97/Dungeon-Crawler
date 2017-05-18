#include "Inventory.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

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