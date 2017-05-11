#ifndef CONTAINERINVENTORY_H
#define CONTAINERINVENTORY_H

#include <vector>

#include "Inventory.h"

class Tile;
class InventoryView;
class WindowSpaceManager;
class InternalWindow;

class ContainerInventory : public Inventory
{
public:
    ContainerInventory(int size);
    virtual ~ContainerInventory();

    virtual std::vector<TileStack*>& contents();
    virtual const InventoryContentRequirement slotContentRequirement(size_t slotId) const;
    virtual std::unique_ptr<InventoryView> createInventoryView(InternalWindow& wnd);
    virtual std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm) const;

    void setSize(int newSize);

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<TileStack*> m_contents;
};

#endif // CONTAINERINVENTORY_H
