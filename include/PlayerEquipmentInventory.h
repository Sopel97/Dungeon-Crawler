#ifndef PLAYEREQUIPMENTINVENTORY_H
#define PLAYEREQUIPMENTINVENTORY_H

#include <vector>
#include <memory>

#include "Inventory.h"
#include "tiles/TileStack.h"

class TileStack;
class WindowSpaceManager;
class InternalWindow;
class InventorySystem;
class InventoryWindow;

class PlayerEquipmentInventory : public Inventory
{
public:
    PlayerEquipmentInventory();

    std::vector<TileStack>& contents() override;
    const std::vector<TileStack>& contents() const override;
    const SlotContentRequirement slotContentRequirement(size_t slotId) const override;
    std::unique_ptr<InventoryView> createInventoryView(InventorySystem& invSys, InternalWindow& wnd) override;
    std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm) const override;

    int size() const override;

protected:
private:
    std::vector<TileStack> m_contents;
    std::vector<SlotContentRequirement> m_contentRequirements;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
