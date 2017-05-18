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
TileStack* Inventory::at(size_t slotId)
{
    return contents()[slotId];
}

void Inventory::deleteTileAt(size_t slotId)
{
    auto& slots = this->contents();
    if(slotId >= slots.size()) return;
    if(slots[slotId])
    {
        delete slots[slotId];
        slots[slotId] = nullptr;
    }
}
int Inventory::insert(TileStack* tileStack, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfInsertedTiles = 0;
    
    auto& slots = this->contents();
    size_t slotId = 0;
    while(slotId < slots.size() && numberOfInsertedTiles < count)
    {
        numberOfInsertedTiles += insert(tileStack, slotId, count - numberOfInsertedTiles);
    }

    return numberOfInsertedTiles;
}
int Inventory::insert(TileStack* tileStack, size_t slotId, int count)
{
    auto& slots = this->contents();
    if(slotId >= slots.size()) return 0;

    if(count == -1) count = std::numeric_limits<int>::max();
    if(count <= 0) return 0;

    TileStack* storedTile = slots[slotId];
    if(storedTile == nullptr)
    {
        storedTile = tileStack->clone().release();
        storedTile->setQuantity(0); //after inserting it won't be 0
    }
    else if(!storedTile->tile().equals(tileStack->tile()))
    {
        return 0;
    }

    int storedTileQuantity = storedTile->quantity();
    int freeSpace = storedTile->maxQuantity() - storedTileQuantity;

    count = std::min(count, freeSpace);

    int newQuantity = storedTileQuantity + count;
    storedTile->setQuantity(newQuantity);

    return count;
}

int Inventory::erase(TileStack* tileStack, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfErasedTiles = 0;

    auto& slots = this->contents();
    size_t slotId = 0;
    while(slotId < slots.size() && numberOfErasedTiles < count)
    {
        numberOfErasedTiles += erase(tileStack, slotId, count - numberOfErasedTiles);
    }

    return numberOfErasedTiles;
}
int Inventory::erase(TileStack* tileStack, size_t slotId, int count)
{
    auto& slots = this->contents();
    if(slotId >= slots.size()) return 0;

    TileStack* storedTile = slots[slotId];
    if(storedTile == nullptr) return 0;
    if(!storedTile->tile().equals(tileStack->tile())) return 0;
    int storedTileQuantity = storedTile->quantity();

    if(count == -1) count = storedTileQuantity;
    int newQuantity = storedTileQuantity - count;
    if(newQuantity <= 0)
    {
        deleteTileAt(slotId);
    }
    else
    {
        storedTile->setQuantity(newQuantity);
    }

    return count;
}