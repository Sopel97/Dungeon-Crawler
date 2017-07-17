#pragma once

#include "InventorySlotView.h"

class TileStack;

class TilePlacedInInventory
{
public:
    const TileStack* tile;
    const Inventory* inventory;
    const int slotId;
};