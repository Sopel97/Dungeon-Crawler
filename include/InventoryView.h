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
    void setOffsetFromTop(int newOffset);

    bool isMinimizable() const;
    bool isCloseable() const;
    bool isResizeable() const;

    void setMinimizable(bool newValue);
    void setCloseable(bool newValue);
    void setResizeable(bool newValue);

    void setInnerHeight(int newInnerHeight);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    static int m_topBarSize;
    static int m_leftMarginSize;
    static int m_bottomMarginSize;
    static int m_rightMarginSize;

    std::vector<InventorySlotView> m_slotViews;

    int m_offsetFromTop;
    int m_scroll;
    int m_height;

    bool m_isMinimizable;
    bool m_isCloseable;
    bool m_isResizeable;
};

#endif // INVENTORYVIEW_H
