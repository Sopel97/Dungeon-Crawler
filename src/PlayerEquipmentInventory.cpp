#include "PlayerEquipmentInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"

#include "Tile.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

PlayerEquipmentInventory::PlayerEquipmentInventory()
{
    m_contents.resize(10, nullptr);
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
InventoryView PlayerEquipmentInventory::createInventoryView()
{
    std::vector<InventorySlotView> slotViews;
    slotViews.reserve(m_contents.size());

    slotViews.emplace_back(m_contents[0], Vec2I(57, 17+43*0), InventorySlotView::ContentRequirement::Necklace);
    slotViews.emplace_back(m_contents[1], Vec2I(57, 17+43*1), InventorySlotView::ContentRequirement::Sword);
    slotViews.emplace_back(m_contents[2], Vec2I(57, 17+43*2), InventorySlotView::ContentRequirement::Ring);

    slotViews.emplace_back(m_contents[3], Vec2I(100, 4+43*0), InventorySlotView::ContentRequirement::Helmet);
    slotViews.emplace_back(m_contents[4], Vec2I(100, 4+43*1), InventorySlotView::ContentRequirement::Chestplate);
    slotViews.emplace_back(m_contents[5], Vec2I(100, 4+43*2), InventorySlotView::ContentRequirement::Pants);
    slotViews.emplace_back(m_contents[6], Vec2I(100, 4+43*3), InventorySlotView::ContentRequirement::Boots);

    slotViews.emplace_back(m_contents[7], Vec2I(143, 17+43*0), InventorySlotView::ContentRequirement::Container);
    slotViews.emplace_back(m_contents[8], Vec2I(143, 17+43*1), InventorySlotView::ContentRequirement::Shield);
    slotViews.emplace_back(m_contents[9], Vec2I(143, 17+43*2), InventorySlotView::ContentRequirement::Ammo);

    InventoryView inventoryView(this, std::move(slotViews));
    inventoryView.setMinimizable(true);
    inventoryView.setCloseable(false);
    inventoryView.setResizeable(false);
    inventoryView.setMaxHeight();

    return inventoryView;
}

int PlayerEquipmentInventory::numberOfSlots() const
{
    return m_contents.size();
}
