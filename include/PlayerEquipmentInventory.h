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
    enum class SlotType
    {
        Necklace = 0,
        PrimaryWeapon,
        Ammo,
        Helmet,
        Chestplate,
        Pants,
        Boots,
        Container,
        SecondaryHand,
        Ring,
        NumSlotTypes
    };

    PlayerEquipmentInventory();

    std::vector<TileStack>& contents() override;
    const std::vector<TileStack>& contents() const override;
    const SlotContentRequirement slotContentRequirement(size_t slotId) const override;
    std::unique_ptr<InventoryView> createInventoryView(InventorySystem& invSys) override;
    std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const override;

    TileStack& at(SlotType slot);
    const TileStack& at(SlotType slot) const;
    int slotId(SlotType slot) const;

    int size() const override;

protected:
private:
    std::vector<TileStack> m_contents;
    std::vector<SlotContentRequirement> m_contentRequirements;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
