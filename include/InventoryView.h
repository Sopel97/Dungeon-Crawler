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
    static WindowSpaceManager::WindowParams defaultParams();

    InventoryWindow(const std::string& name);
    InventoryWindow(const std::string& name, const WindowSpaceManager::WindowParams& params);

    virtual int verticalScroll() const;

    void setContentHeightToMax();

    void update();

    void updateMaxContentHeight(const InventoryView& view);

    WindowSpaceManager::WindowFullLocalization fullLocalization();

protected:
    static constexpr int m_minSensibleHeight = 46;

    int m_scroll;
};
