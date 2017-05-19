#include "InventoryView.h"

#include "Root.h"

#include "ResourceManager.h"

#include "InventorySystem.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

InventoryView::InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory) :
    WindowContent(wnd),
    m_inventorySystem(&invSys),
    m_inventory(&inventory)
{
}
InventoryView::InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory, const std::vector<InventorySlotView>& slots) :
    WindowContent(wnd),
    m_inventorySystem(&invSys),
    m_inventory(&inventory),
    m_slotViews(slots)
{
}
InventoryView::InventoryView(InventorySystem& invSys, InternalWindow& wnd, Inventory& inventory, std::vector<InventorySlotView>&& slots) :
    WindowContent(wnd),
    m_inventorySystem(&invSys),
    m_inventory(&inventory),
    m_slotViews(std::move(slots))
{
}

void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
}

const std::vector<InventorySlotView>& InventoryView::slotViews() const
{
    return m_slotViews;
}

Inventory& InventoryView::inventory() const
{
    return *m_inventory;
}

const InventorySlotView* InventoryView::querySlot(const ls::Vec2I& pos) const
{
    const ls::Vec2I slotSize = InventorySlotView::slotSize();

    for (const auto& slot : m_slotViews)
    {
        if (ls::intersect(ls::Rectangle2I::withSize(slot.position(), slotSize.x, slotSize.y), pos))
        {
            return &slot;
        }
    }

    return nullptr;
}
InventorySlotView* InventoryView::querySlot(const ls::Vec2I& pos)
{
    return const_cast<InventorySlotView*>(const_cast<const InventoryView*>(this)->querySlot(pos));
}
SfmlEventHandler::EventResult InventoryView::onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context)
{
    if (!context.isMouseOver) return EventResult{}.setTakeFocus(false).setConsumeEvent(false);

    const ls::Vec2I pos = window().localContentCoords(ls::Vec2I(event.x, event.y));
    InventorySlotView* slot = querySlot(pos);
    if (slot != nullptr)
    {
        std::cout << m_inventory << ' ' << slot->slotId() << '\n';
    }
    return EventResult{}.setTakeFocus().setConsumeEvent();
}

void InventoryView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int numberOfSlots = m_slotViews.size();
    for (int i = 0; i < numberOfSlots; ++i)
    {
        m_slotViews[i].draw(renderTarget, renderStates, m_inventory->slotContentRequirement(i));
    }
}
