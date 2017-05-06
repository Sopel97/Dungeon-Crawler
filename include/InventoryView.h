#pragma once

#include "InventorySlotView.h"
#include "WindowSpaceManager.h"
#include "WindowSpaceUser.h"

#include "PlayerUi.h"

#include <vector>

class Inventory;

class InventoryView : public WindowSpaceUser
{
private:
    Inventory* m_inventory;
    std::vector<InventorySlotView> m_slotViews;

public:
    InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory);
    InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory, const std::vector<InventorySlotView>& slots);
    InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory, std::vector<InventorySlotView>&& slots);

    InventoryView(const InventoryView& other) = delete;
    InventoryView(InventoryView&& other) = default;

    InventoryView& operator =(const InventoryView& other) = delete;
    InventoryView& operator =(InventoryView&& other) = default;

    void addInventorySlotView(const InventorySlotView& slot);

    const std::vector<InventorySlotView>& slotViews() const;
    Inventory& inventory() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
};

class InventoryWindow : public PlayerUi::PanelWindow
{
public:
    InventoryWindow(const std::string& name);

    InventoryWindow(const InventoryWindow& other) = default;
    InventoryWindow(InventoryWindow&& other) = default;

    InventoryWindow& operator =(const InventoryWindow& other) = default;
    InventoryWindow& operator =(InventoryWindow&& other) = default;

    virtual ls::Vec2I minWindowSize() const;
    virtual bool hasMaxWindowSize() const;
    virtual ls::Vec2I maxWindowSize() const;
    virtual ls::Vec2I minContentSize() const;
    virtual bool hasMaxContentSize() const;
    virtual ls::Vec2I maxContentSize() const;

    virtual int verticalScroll() const;

    virtual bool isMinimizable() const;
    virtual bool isCloseable() const;
    virtual bool isResizeable() const;
    virtual bool isMovable() const;

    virtual bool hasHeader() const;
    virtual bool hasScrollBar() const;

    void setMinimizable(bool newValue);
    void setCloseable(bool newValue);
    void setResizeable(bool newValue);
    void setMovable(bool newValue);
    void setHeaderEnabled(bool newValue);
    void setScrollBarEnabled(bool doEnable);

    void setContentSizeToMax();

    void update();

    void updateMaxContentHeight(const InventoryView& view);

    WindowSpaceManager::WindowFullLocalization fullLocalization();

protected:
    static const int m_minSensibleHeight;

    int m_scroll;
    int m_minContentHeight;
    int m_maxContentHeight;

    bool m_isMinimizable;
    bool m_isCloseable;
    bool m_isResizeable;
    bool m_isMovable;
    bool m_hasHeader;
    bool m_hasScrollBar;
};
