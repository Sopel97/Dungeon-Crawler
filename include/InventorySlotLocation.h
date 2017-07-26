#pragma once

class Inventory;

class InventorySlotLocation
{
public:
    const Inventory* inventory;
    const int slotId;

    InventorySlotLocation(const Inventory& inventory_, int slotId_);
};