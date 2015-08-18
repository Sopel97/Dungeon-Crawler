#ifndef PLAYEREQUIPMENTINVENTORY_H
#define PLAYEREQUIPMENTINVENTORY_H

#include "Inventory.h"

#include <vector>

class Tile;

class PlayerEquipmentInventory : public Inventory
{
public:
    PlayerEquipmentInventory();
    virtual ~PlayerEquipmentInventory();

    virtual const std::vector<Tile*>& contents() const;
    virtual InventoryView createInventoryView();

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<Tile*> m_contents;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
