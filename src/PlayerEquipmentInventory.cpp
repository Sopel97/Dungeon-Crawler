#include "PlayerEquipmentInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"
#include "window/InventoryWindow.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "../LibS/Geometry.h"

using namespace ls;

PlayerEquipmentInventory::PlayerEquipmentInventory()
{
    m_contents.resize(10);
    m_contentRequirements.resize(10);

    m_contentRequirements[static_cast<int>(SlotType::Necklace)]      = SlotContentRequirement::Necklace;
    m_contentRequirements[static_cast<int>(SlotType::PrimaryWeapon)] = SlotContentRequirement::RightHand;
    m_contentRequirements[static_cast<int>(SlotType::Ammo)]          = SlotContentRequirement::Ammo;
    m_contentRequirements[static_cast<int>(SlotType::Helmet)]        = SlotContentRequirement::Helmet;
    m_contentRequirements[static_cast<int>(SlotType::Chestplate)]    = SlotContentRequirement::Chestplate;
    m_contentRequirements[static_cast<int>(SlotType::Pants)]         = SlotContentRequirement::Pants;
    m_contentRequirements[static_cast<int>(SlotType::Boots)]         = SlotContentRequirement::Boots;
    m_contentRequirements[static_cast<int>(SlotType::Container)]     = SlotContentRequirement::Container;
    m_contentRequirements[static_cast<int>(SlotType::SecondaryHand)] = SlotContentRequirement::LeftHand;
    m_contentRequirements[static_cast<int>(SlotType::Ring)]          = SlotContentRequirement::Ring;
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
    slotViews.reserve(m_contents.size());

    slotViews.emplace_back(this, 0, Vec2I(52, 17+43*0));
    slotViews.emplace_back(this, 1, Vec2I(52, 17+43*1));
    slotViews.emplace_back(this, 2, Vec2I(52, 17+43*2));

    slotViews.emplace_back(this, 3, Vec2I(95, 4+43*0));
    slotViews.emplace_back(this, 4, Vec2I(95, 4+43*1));
    slotViews.emplace_back(this, 5, Vec2I(95, 4+43*2));
    slotViews.emplace_back(this, 6, Vec2I(95, 4+43*3));

    slotViews.emplace_back(this, 7, Vec2I(138, 17+43*0));
    slotViews.emplace_back(this, 8, Vec2I(138, 17+43*1));
    slotViews.emplace_back(this, 9, Vec2I(138, 17+43*2));

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
