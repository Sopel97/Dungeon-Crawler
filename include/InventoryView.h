#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include "InventorySlotView.h"

#include <vector>

class Inventory;

class InventoryView
{
public:
    InventoryView(Inventory* parent);
    InventoryView(Inventory* parent, const std::vector<InventorySlotView>& slots);
    InventoryView(Inventory* parent, std::vector<InventorySlotView>&& slots);

    InventoryView(const InventoryView&) = default;
    InventoryView(InventoryView&&) = default;

    InventoryView& operator =(const InventoryView&) = default;
    InventoryView& operator =(InventoryView&&) = default;

    void addInventorySlotView(const InventorySlotView& slot);
    void setOffsetFromTop(int newOffset);

    Inventory const* parentInventory() const;
    int offsetFromTop() const;
    int height() const;
    int minHeight() const;
    int maxHeight() const;

    bool isMinimizable() const;
    bool isCloseable() const;
    bool isResizeable() const;

    void setMinimizable(bool newValue);
    void setCloseable(bool newValue);
    void setResizeable(bool newValue);

    void setHeight(int newHeight);
    void setInnerHeight(int newInnerHeight);
    void setHeightToMax();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void update();

protected:
    static int m_minSensibleHeight;
    static int m_topBarHeight;
    static int m_leftBarWidth;
    static int m_bottomBarHeight;
    static int m_rightBarWidth;

    Inventory* m_parentInventory;
    std::vector<InventorySlotView> m_slotViews;

    int m_offsetFromTop;
    int m_scroll;
    int m_height;
    int m_minHeight;
    int m_maxHeight;

    bool m_isMinimizable;
    bool m_isCloseable;
    bool m_isResizeable;

    void updateMaxHeight();
};

#endif // INVENTORYVIEW_H
