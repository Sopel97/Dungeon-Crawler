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

    virtual std::vector<Tile*>& contents();
    virtual const Inventory::ContentRequirement slotContentRequirement(size_t slotId) const;
    virtual InventoryView createInventoryView();

    void setSize(int newSize);

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<Tile*> m_contents;
};

#endif // CONTAINERINVENTORY_H
