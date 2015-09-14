#include "PlayerEquipmentInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"

#include "tiles/Tile.h"

#include "../LibS/Geometry.h"

using namespace ls;

PlayerEquipmentInventory::PlayerEquipmentInventory()
{
    m_contents.resize(10, nullptr);
    m_contentRequirements.reserve(10);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Necklace);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Sword);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Ring);

    m_contentRequirements.push_back(Inventory::ContentRequirement::Helmet);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Chestplate);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Pants);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Boots);

    m_contentRequirements.push_back(Inventory::ContentRequirement::Container);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Shield);
    m_contentRequirements.push_back(Inventory::ContentRequirement::Ammo);
}
PlayerEquipmentInventory::~PlayerEquipmentInventory()
{
    for(Tile* tile : m_contents)
    {
        if(tile != nullptr)
            delete tile;
    }
}

const std::vector<Tile*>& PlayerEquipmentInventory::contents() const
{
    return m_contents;
}
const Inventory::ContentRequirement PlayerEquipmentInventory::slotContentRequirement(int slotId) const
{
    return m_contentRequirements[slotId];
}
InventoryView PlayerEquipmentInventory::createInventoryView()
{
    std::vector<InventorySlotView> slotViews;
    slotViews.reserve(m_contents.size());

    slotViews.emplace_back(m_contents[0], Vec2I(52, 17+43*0));
    slotViews.emplace_back(m_contents[1], Vec2I(52, 17+43*1));
    slotViews.emplace_back(m_contents[2], Vec2I(52, 17+43*2));

    slotViews.emplace_back(m_contents[3], Vec2I(95, 4+43*0));
    slotViews.emplace_back(m_contents[4], Vec2I(95, 4+43*1));
    slotViews.emplace_back(m_contents[5], Vec2I(95, 4+43*2));
    slotViews.emplace_back(m_contents[6], Vec2I(95, 4+43*3));

    slotViews.emplace_back(m_contents[7], Vec2I(138, 17+43*0));
    slotViews.emplace_back(m_contents[8], Vec2I(138, 17+43*1));
    slotViews.emplace_back(m_contents[9], Vec2I(138, 17+43*2));

    InventoryView inventoryView(this, std::move(slotViews));
    inventoryView.setMinimizable(true);
    inventoryView.setCloseable(false);
    inventoryView.setResizeable(false);
    inventoryView.setContentHeightToMax();

    return inventoryView;
}

int PlayerEquipmentInventory::numberOfSlots() const
{
    return m_contents.size();
}
