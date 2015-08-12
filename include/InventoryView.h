#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include "InventorySlotView.h"

#include <vector>

class InventoryView
{
public:
    InventoryView();
    InventoryView(const std::vector<InventorySlotView>& slots);
    InventoryView(std::vector<InventorySlotView>&& slots);

    InventoryView(const InventoryView&) = default;
    InventoryView(InventoryView&&) = default;

    InventoryView& operator =(const InventoryView&) = default;
    InventoryView& operator =(InventoryView&&) = default;

    void addInventorySlotView(const InventorySlotView& slot);
    void setOffset(int newOffset);

protected:
    std::vector<InventorySlotView> m_slotViews;

    int m_offset; //offset from top
};

#endif // INVENTORYVIEW_H
