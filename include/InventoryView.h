#pragma once

#include "InventorySlotView.h"
#include "WindowSpaceManager.h"
#include "WindowContent.h"

#include "PlayerUi.h"

#include <vector>

class Inventory;

class InventoryView : public WindowContent
{
private:
    Inventory* m_inventory;
    std::vector<InventorySlotView> m_slotViews;

public:
    InventoryView(WindowSpaceManager::Window& wnd, Inventory& inventory);
    InventoryView(WindowSpaceManager::Window& wnd, Inventory& inventory, const std::vector<InventorySlotView>& slots);
    InventoryView(WindowSpaceManager::Window& wnd, Inventory& inventory, std::vector<InventorySlotView>&& slots);

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
    static WindowSpaceManager::WindowParams defaultParams();

    InventoryWindow(WindowSpaceManager& wsm, const std::string& name);
    InventoryWindow(WindowSpaceManager& wsm, const std::string& name, const WindowSpaceManager::WindowParams& params);

    virtual int verticalScroll() const;

    void setContentHeightToMax();

    void update();

    void updateMaxContentHeight(const InventoryView& view);

protected:
    static constexpr int m_minSensibleHeight = 46;

    int m_scroll;
};
