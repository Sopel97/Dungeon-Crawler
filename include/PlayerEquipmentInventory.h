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
        Weapon,
        Ring1,
        Ring2,
        Helmet,
        Chestplate,
        Pants,
        Boots,
        Container,
        Shield,
        Ammo,
        LightSource,
        NumSlotTypes
    };

    PlayerEquipmentInventory();

    std::vector<TileStack>& contents() override;
    const std::vector<TileStack>& contents() const override;
    const SlotContentRequirement slotContentRequirement(int slotId) const override;
    std::unique_ptr<InventoryView> createInventoryView(InventorySystem& invSys) override;
    std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const override;

    using Inventory::at;
    TileStack& at(SlotType slot);
    const TileStack& at(SlotType slot) const;
    int slotId(SlotType slot) const;

protected:
private:
    struct SlotData
    {
        SlotType id;
        SlotContentRequirement req;
        ls::Vec2I pos;
    };

    static const std::vector<SlotData> m_slots;

    std::vector<TileStack> m_contents;
    std::vector<SlotContentRequirement> m_contentRequirements;
};

#endif // PLAYEREQUIPMENTINVENTORY_H
