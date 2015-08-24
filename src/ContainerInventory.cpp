#include "ContainerInventory.h"

#include "InventorySlotView.h"
#include "InventoryView.h"

#include "../LibS/Geometry.h"

using namespace ls;

ContainerInventory::ContainerInventory(int size)
{
    m_contents.resize(size, nullptr);
}

ContainerInventory::~ContainerInventory()
{
    //dtor
}

const std::vector<Tile*>& ContainerInventory::contents() const
{
    return m_contents;
}
InventoryView ContainerInventory::createInventoryView()
{
    constexpr int margins = 4;
    constexpr int spacesBetweenSlots = 3;
    const int slotCount = m_contents.size();

    std::vector<InventorySlotView> slotViews;
    slotViews.reserve(slotCount);

    int row = 0;
    int col = 0;
    for(int i = 0; i < slotCount; ++i)
    {
        slotViews.emplace_back(m_contents[i], Vec2F(margins + (InventorySlotView::slotSize().x + spacesBetweenSlots)*col, margins + (InventorySlotView::slotSize().y + spacesBetweenSlots)*row), InventorySlotView::ContentRequirement::None);

        ++col;
        if(col == 5)
        {
            ++row;
            col = 0;
        }
    }

    InventoryView inventoryView(this, std::move(slotViews));
    inventoryView.setMinimizable(true);
    inventoryView.setCloseable(true);
    inventoryView.setResizeable(true);
    inventoryView.setMaxHeight();

    return inventoryView;
}

int ContainerInventory::numberOfSlots() const
{
    return m_contents.size();
}
