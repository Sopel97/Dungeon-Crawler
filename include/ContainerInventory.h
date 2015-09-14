#ifndef CONTAINERINVENTORY_H
#define CONTAINERINVENTORY_H

#include <vector>

#include "Inventory.h"

class Tile;
class InventoryView;

class ContainerInventory : public Inventory
{
public:
    ContainerInventory(int size);
    virtual ~ContainerInventory();

    virtual const std::vector<Tile*>& contents() const;
    virtual const Inventory::ContentRequirement slotContentRequirement(int slotId) const;
    virtual InventoryView createInventoryView();

    void setSize(int newSize);

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<Tile*> m_contents;
};

#endif // CONTAINERINVENTORY_H
