#include "ContainerInventory.h"

#include "../LibS/Geometry.h"

#include "tiles/TileStack.h"

#include "InventorySlotView.h"
#include "InventoryView.h"

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
const InventoryContentRequirement ContainerInventory::slotContentRequirement(size_t slotId) const
{
    return InventoryContentRequirement::None;
}
std::unique_ptr<InventoryView> ContainerInventory::createInventoryView(InternalWindow& wnd)
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

    return std::make_unique<InventoryView>(wnd, *this, std::move(slotViews));
}
std::unique_ptr<InventoryWindow> ContainerInventory::createInventoryWindow(WindowSpaceManager& wsm) const
{
    std::unique_ptr<InventoryWindow> window = std::make_unique<InventoryWindow>(wsm, "");
    window->setMinimizable(true);
    window->setCloseable(true);
    window->setResizeable(true);
    window->setMovable(true);
    window->setContentOnly(false);
    window->setHeaderEnabled(true);
    window->setScrollBarEnabled(true);
    return window;
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
