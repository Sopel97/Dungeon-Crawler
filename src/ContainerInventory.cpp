#include "ContainerInventory.h"

#include "tiles/TileStack.h"

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

std::vector<TileStack*>& ContainerInventory::contents()
{
    return m_contents;
}
const Inventory::ContentRequirement ContainerInventory::slotContentRequirement(size_t slotId) const
{
    return Inventory::ContentRequirement::None;
}
InventoryView ContainerInventory::createInventoryView()
{
    constexpr int margins = 4;
    constexpr int spacesBetweenSlots = 3;
    const size_t slotCount = m_contents.size();

    std::vector<InventorySlotView> slotViews;
    slotViews.reserve(slotCount);

    int row = 0;
    int col = 0;
    for(size_t i = 0; i < slotCount; ++i)
    {
        slotViews.emplace_back(this, i, Vec2I(margins + (InventorySlotView::slotSize().x + spacesBetweenSlots)*col, margins + (InventorySlotView::slotSize().y + spacesBetweenSlots)*row));

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
    inventoryView.setContentHeightToMax();

    return inventoryView;
}

void ContainerInventory::setSize(int newSize)
{
    for(size_t i = newSize; i < m_contents.size(); ++i) delete m_contents[i];
    m_contents.resize(newSize, nullptr);
}

int ContainerInventory::numberOfSlots() const
{
    return m_contents.size();
}
