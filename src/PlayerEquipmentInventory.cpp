#include "PlayerEquipmentInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"
#include "window/InventoryWindow.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "../LibS/Geometry.h"

using namespace ls;

const std::vector<PlayerEquipmentInventory::SlotData> PlayerEquipmentInventory::m_slots{
    { SlotType::Necklace,    SlotContentRequirement::Necklace,    Vec2I(52 + 43 * 0, 4 + 43 * 0) },
    { SlotType::Weapon,      SlotContentRequirement::Weapon,      Vec2I(52 + 43 * 0, 4 + 43 * 1) },
    { SlotType::Ring1,       SlotContentRequirement::Ring,        Vec2I(52 + 43 * 0, 4 + 43 * 2) },
    { SlotType::Ring2,       SlotContentRequirement::Ring,        Vec2I(52 + 43 * 0, 4 + 43 * 3) },

    { SlotType::Helmet,      SlotContentRequirement::Helmet,      Vec2I(52 + 43 * 1, 4 + 43 * 0) },
    { SlotType::Chestplate,  SlotContentRequirement::Chestplate,  Vec2I(52 + 43 * 1, 4 + 43 * 1) },
    { SlotType::Pants,       SlotContentRequirement::Pants,       Vec2I(52 + 43 * 1, 4 + 43 * 2) },
    { SlotType::Boots,       SlotContentRequirement::Boots,       Vec2I(52 + 43 * 1, 4 + 43 * 3) },

    { SlotType::Container,   SlotContentRequirement::Container,   Vec2I(52 + 43 * 2, 4 + 43 * 0) },
    { SlotType::Shield,      SlotContentRequirement::Shield,      Vec2I(52 + 43 * 2, 4 + 43 * 1) },
    { SlotType::Ammo,        SlotContentRequirement::Ammo,        Vec2I(52 + 43 * 2, 4 + 43 * 2) },
    { SlotType::LightSource, SlotContentRequirement::LightSource, Vec2I(52 + 43 * 2, 4 + 43 * 3) }
};

PlayerEquipmentInventory::PlayerEquipmentInventory()
{
    m_contents.resize(m_slots.size());
    m_contentRequirements.resize(m_slots.size());

    for (const auto& slot : m_slots)
    {
        m_contentRequirements[static_cast<int>(slot.id)] = slot.req;
    }
}

std::vector<TileStack>& PlayerEquipmentInventory::contents()
{
    return m_contents;
}
const std::vector<TileStack>& PlayerEquipmentInventory::contents() const
{
    return m_contents;
}
const SlotContentRequirement PlayerEquipmentInventory::slotContentRequirement(int slotId) const
{
    return m_contentRequirements[slotId];
}
std::unique_ptr<InventoryView> PlayerEquipmentInventory::createInventoryView(InventorySystem& invSys)
{
    std::vector<InventorySlotView> slotViews;
    slotViews.reserve(m_slots.size());

    for (const auto& slot : m_slots)
    {
        slotViews.emplace_back(this, static_cast<int>(slot.id), slot.pos);
    }

    return std::make_unique<InventoryView>(invSys, *this, std::move(slotViews));
}

std::unique_ptr<InventoryWindow> PlayerEquipmentInventory::createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const
{
    std::unique_ptr<InventoryWindow> window = std::make_unique<InventoryWindow>(wsm, name);
    window->setMinimizable(true);
    window->setCloseable(false);
    window->setResizeable(false);
    window->setMovable(false);
    window->setContentOnly(false);
    window->setHeaderEnabled(false);
    window->setScrollBarEnabled(false);
    return window;
}
TileStack& PlayerEquipmentInventory::at(SlotType slot)
{
    return m_contents[static_cast<int>(slot)];
}
const TileStack& PlayerEquipmentInventory::at(SlotType slot) const
{
    return m_contents[static_cast<int>(slot)];
}
int PlayerEquipmentInventory::slotId(SlotType slot) const
{
    return static_cast<int>(slot);
}
