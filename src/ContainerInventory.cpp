#include "ContainerInventory.h"

#include "../LibS/Geometry.h"

#include "tiles/TileStack.h"

#include "InventorySlotView.h"
#include "InventoryView.h"
#include "window/InventoryWindow.h"

using namespace ls;

ContainerInventory::ContainerInventory(int size) :
    m_contents(size)
{

}
ContainerInventory::ContainerInventory(const ContainerInventory& other) :
    ContainerInventory(other.size())
{
    for (int i = 0; i < other.size(); ++i)
    {
        m_contents.at(i) = other.at(i).clone();
    }
}

std::vector<TileStack>& ContainerInventory::contents()
{
    return m_contents;
}
const std::vector<TileStack>& ContainerInventory::contents() const
{
    return m_contents;
}

const SlotContentRequirement ContainerInventory::slotContentRequirement(size_t slotId) const
{
    return SlotContentRequirement::None;
}
std::unique_ptr<InventoryView> ContainerInventory::createInventoryView(InventorySystem& invSys)
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

    return std::make_unique<InventoryView>(invSys, *this, std::move(slotViews));
}
std::unique_ptr<InventoryWindow> ContainerInventory::createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const
{
    std::unique_ptr<InventoryWindow> window = std::make_unique<InventoryWindow>(wsm, name);
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
    m_contents.resize(newSize);
}

int ContainerInventory::size() const
{
    return m_contents.size();
}
