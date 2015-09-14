#include "Inventory.h"

#include "tiles/Tile.h"

Inventory::Inventory()
{

}
Inventory::~Inventory()
{

}

int Inventory::insert(Tile* tile, int count)
{
    if(count == -1) count = tile->quantity();

    return count;
}
int Inventory::insert(Tile* tile, int slotId, int count)
{
    if(count == -1) count = tile->quantity();

    return count;
}

int Inventory::erase(Tile* tile, int count)
{
    if(count == -1) count = tile->quantity();

    return count;
}
int Inventory::erase(Tile* tile, int slotId, int count)
{
    if(count == -1) count = tile->quantity();

    return count;
}