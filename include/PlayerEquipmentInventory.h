#ifndef PLAYEREQUIPMENTINVENTORY_H
#define PLAYEREQUIPMENTINVENTORY_H

#include "Inventory.h"

#include <vector>

class TileStack;
class WindowSpaceManager;
class InternalWindow;

class PlayerEquipmentInventory : public Inventory
{
public:
    PlayerEquipmentInventory();
    virtual ~PlayerEquipmentInventory();

    virtual std::vector<TileStack*>& contents();
    virtual const InventoryContentRequirement slotContentRequirement(size_t slotId) const;
    virtual std::unique_ptr<InventoryView> createInventoryView(InternalWindow& wnd);
    virtual std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm) const;

    virtual int numberOfSlots() const;

protected:
private:
    std::vector<TileStack*> m_contents;
    std::vector<InventoryContentRequirement> m_contentRequirements;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
