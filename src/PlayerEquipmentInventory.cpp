#include "PlayerEquipmentInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"

#include "Tile.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

PlayerEquipmentInventory::PlayerEquipmentInventory()
{
    m_contents.resize(9);
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

    slotViews.emplace_back(m_contents[0], Vec2I(57, 17+43*0), InventorySlotView::ContentRequirement::Necklace);
    slotViews.emplace_back(m_contents[0], Vec2I(57, 17+43*1), InventorySlotView::ContentRequirement::Hand);
    slotViews.emplace_back(m_contents[0], Vec2I(57, 17+43*2), InventorySlotView::ContentRequirement::Ring);

    slotViews.emplace_back(m_contents[0], Vec2I(100, 4+43*0), InventorySlotView::ContentRequirement::Head);
    slotViews.emplace_back(m_contents[0], Vec2I(100, 4+43*1), InventorySlotView::ContentRequirement::Chest);
    slotViews.emplace_back(m_contents[0], Vec2I(100, 4+43*2), InventorySlotView::ContentRequirement::Legs);
    slotViews.emplace_back(m_contents[0], Vec2I(100, 4+43*3), InventorySlotView::ContentRequirement::Feet);

    slotViews.emplace_back(m_contents[0], Vec2I(143, 17+43*0), InventorySlotView::ContentRequirement::Container);
    slotViews.emplace_back(m_contents[0], Vec2I(143, 17+43*1), InventorySlotView::ContentRequirement::Hand);
    slotViews.emplace_back(m_contents[0], Vec2I(143, 17+43*2), InventorySlotView::ContentRequirement::Ammo);

    InventoryView inventoryView(std::move(slotViews));
    inventoryView.setMinimizable(true);
    inventoryView.setCloseable(false);
    inventoryView.setResizeable(false);

    return inventoryView;
}

int PlayerEquipmentInventory::numberOfSlots() const
{
    return m_contents.size();
}

bool PlayerEquipmentInventory::isOpened() const
{
    return m_isOpened;
}
void PlayerEquipmentInventory::onOpened()
{
    m_isOpened = true;
}
void PlayerEquipmentInventory::onClosed()
{
    m_isOpened = false;
}
