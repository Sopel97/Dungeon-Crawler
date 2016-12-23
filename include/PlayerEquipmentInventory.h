#ifndef PLAYEREQUIPMENTINVENTORY_H
#define PLAYEREQUIPMENTINVENTORY_H

#include "Inventory.h"

#include <vector>

class TileStack;

class PlayerEquipmentInventory : public Inventory
{
public:
    PlayerEquipmentInventory();
    virtual ~PlayerEquipmentInventory();

    virtual std::vector<TileStack*>& contents();
    virtual const Inventory::ContentRequirement slotContentRequirement(size_t slotId) const;
    virtual InventoryView createInventoryView();

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<TileStack*> m_contents;
    std::vector<Inventory::ContentRequirement> m_contentRequirements;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
