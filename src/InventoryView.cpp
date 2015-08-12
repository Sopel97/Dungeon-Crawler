#include "InventoryView.h"

InventoryView::InventoryView() :
    m_offset(0)
{

}
InventoryView::InventoryView(const std::vector<InventorySlotView>& slots) :
    m_slotViews(slots),
    m_offset(0)
{

}
InventoryView::InventoryView(std::vector<InventorySlotView>&& slots) :
    m_slotViews(std::move(slots)),
    m_offset(0)
{

}

void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
}

void InventoryView::setOffset(int newOffset)
{
    m_offset = newOffset;
}
