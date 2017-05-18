#pragma once

#include "InventorySlotView.h"
#include "window/WindowSpaceManager.h"
#include "window/WindowContent.h"
#include "window/InternalWindow.h"

#include "PlayerUi.h"

#include <vector>

class Inventory;
class InventorySystem;

class InventoryView : public WindowContent
{
private:
    InventorySystem* m_inventorySystem;
    Inventory* m_inventory;
    std::vector<InventorySlotView> m_slotViews;

public:
    InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory);
    InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory, const std::vector<InventorySlotView>& slots);
    InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory, std::vector<InventorySlotView>&& slots);

    InventoryView(const InventoryView& other) = delete;
    InventoryView(InventoryView&& other) = default;

    InventoryView& operator =(const InventoryView& other) = delete;
    InventoryView& operator =(InventoryView&& other) = default;

    void addInventorySlotView(const InventorySlotView& slot);

    const std::vector<InventorySlotView>& slotViews() const;
    Inventory& inventory() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
};
