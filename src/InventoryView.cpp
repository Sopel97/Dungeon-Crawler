#include "InventoryView.h"

InventoryView::InventoryView() :
    m_offset(0),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(const std::vector<InventorySlotView>& slots) :
    m_slotViews(slots),
    m_offset(0),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(std::vector<InventorySlotView>&& slots) :
    m_slotViews(std::move(slots)),
    m_offset(0),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
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

bool InventoryView::isMinimizable() const
{
    return m_isMinimizable;
}
bool InventoryView::isCloseable() const
{
    return m_isCloseable;
}
bool InventoryView::isResizeable() const
{
    return m_isResizeable;
}

void InventoryView::setMinimizable(bool newValue)
{
    m_isMinimizable = newValue;
}
void InventoryView::setCloseable(bool newValue)
{
    m_isCloseable = newValue;
}
void InventoryView::setResizeable(bool newValue)
{
    m_isResizeable = newValue;
}
