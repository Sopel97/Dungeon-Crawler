#pragma once

#include "InventorySlotView.h"

#include "PlayerUi.h"

#include <vector>

class Inventory;

class InventoryView : public PlayerUi::PanelWindow
{
public:
    InventoryView(Inventory* parent);
    InventoryView(Inventory* parent, const std::vector<InventorySlotView>& slots);
    InventoryView(Inventory* parent, std::vector<InventorySlotView>&& slots);

    InventoryView(const InventoryView& other) = default;
    InventoryView(InventoryView&& other) = default;

    InventoryView& operator =(const InventoryView& other) = default;
    InventoryView& operator =(InventoryView&& other) = default;

    void addInventorySlotView(const InventorySlotView& slot);

    Inventory const* parentInventory() const;
    virtual int contentHeight() const;
    virtual int minContentHeight() const;
    virtual int maxContentHeight() const;

    int size() const;

    virtual int scroll() const;

    virtual bool isMinimizable() const;
    virtual bool isCloseable() const;
    virtual bool isResizeable() const;

    void setMinimizable(bool newValue);
    void setCloseable(bool newValue);
    void setResizeable(bool newValue);

    void setContentHeight(int newHeight);
    void setContentHeightToMax();

    virtual void drawContent(PlayerUi& playerUi, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void update();

protected:
    static const int m_minSensibleHeight;

    Inventory* m_parentInventory;
    std::vector<InventorySlotView> m_slotViews;

    int m_scroll;
    int m_contentHeight;
    int m_minContentHeight;
    int m_maxContentHeight;

    bool m_isMinimizable;
    bool m_isCloseable;
    bool m_isResizeable;

    void updateMaxContentHeight();
};
