#include "InventoryView.h"

#include "Root.h"

#include "ResourceManager.h"

#include "InventorySystem.h"
#include "TileTransferMediator.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

#include "Logger.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

InventoryView::InventoryView(InventorySystem& invSys, Inventory& inventory) :
    m_inventorySystem(&invSys),
    m_inventory(&inventory)
{
}
InventoryView::InventoryView(InventorySystem& invSys, Inventory& inventory, const std::vector<InventorySlotView>& slots) :
    m_inventorySystem(&invSys),
    m_inventory(&inventory),
    m_slotViews(slots)
{
}
InventoryView::InventoryView(InventorySystem& invSys, Inventory& inventory, std::vector<InventorySlotView>&& slots) :
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
        if(event.button == sf::Mouse::Button::Left)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt))
            {
                if (!slot->content().isEmpty())
                {
                    slot->content().tile().model().look(m_inventorySystem->player(), *slot);
                }
            }
            else
            {
                m_inventorySystem->tileTransferMediator().grabFromInventory(*m_inventorySystem, *m_inventory, slot->slotId());
            }

            return EventResult{}.setTakeFocus().setConsumeEvent();
        }
        else if (event.button == sf::Mouse::Button::Right)
        {
            if (!slot->content().isEmpty())
            {
                Tile& tile = slot->content().tile();
                tile.model().use(m_inventorySystem->player(), *slot);
            }
        }
    }
    return EventResult{}.setTakeFocus().setConsumeEvent();
}
SfmlEventHandler::EventResult InventoryView::onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context)
{
    if (!context.isMouseOver)
    {
        if (event.button == sf::Mouse::Button::Left)
        {
            Logger::instance().log(Logger::Priority::Debug, "reset tile move from inventory view");
            m_inventorySystem->tileTransferMediator().reset();
        }

        return EventResult{}.setTakeFocus(false).setConsumeEvent(false);
    }

    const ls::Vec2I pos = window().localContentCoords(ls::Vec2I(event.x, event.y));
    InventorySlotView* slot = querySlot(pos);
    if (slot != nullptr)
    {
        if (event.button == sf::Mouse::Button::Left)
        {
            m_inventorySystem->tileTransferMediator().putToInventory(*m_inventorySystem, *m_inventory, slot->slotId());

            return EventResult{}.setTakeFocus().setConsumeEvent();
        }
    }
    return EventResult{}.setTakeFocus().setConsumeEvent();
}

void InventoryView::onDetachedAndWindowClosing(InternalWindow& wnd)
{
    WindowContent::onDetachedAndWindowClosing(wnd);

    m_inventorySystem->onInventoryWindowClosed(*m_inventory);
}

void InventoryView::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    int numberOfSlots = m_slotViews.size();
    for (int i = 0; i < numberOfSlots; ++i)
    {
        m_slotViews[i].draw(renderTarget, renderStates);
    }
}
