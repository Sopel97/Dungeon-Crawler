#pragma once

#include "InventorySlotView.h"

class TileStack;

class TileRemovedFromInventory
{
public:
    const TileStack* tile;
    const Inventory* inventory;
    const int slotId;
};