#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

const int InventoryView::m_minSensibleHeight = 46;

InventoryView::InventoryView(Inventory* parent) :
    m_parentInventory(parent),
    m_scroll(0),
    m_contentHeight(m_minSensibleHeight),
    m_minContentHeight(m_minSensibleHeight),
    m_maxContentHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(Inventory* parent, const std::vector<InventorySlotView>& slots) :
    m_parentInventory(parent),
    m_slotViews(slots),
    m_scroll(0),
    m_contentHeight(m_minSensibleHeight),
    m_minContentHeight(m_minSensibleHeight),
    m_maxContentHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{
    updateMaxContentHeight();
}
InventoryView::InventoryView(Inventory* parent, std::vector<InventorySlotView>&& slots) :
    m_parentInventory(parent),
    m_slotViews(std::move(slots)),
    m_scroll(0),
    m_contentHeight(m_minSensibleHeight),
    m_minContentHeight(m_minSensibleHeight),
    m_maxContentHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{
    updateMaxContentHeight();
}


void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
    updateMaxContentHeight();
}

Inventory const* InventoryView::parentInventory() const
{
    return m_parentInventory;
}
int InventoryView::contentHeight() const
{
    return m_contentHeight;
}
int InventoryView::minContentHeight() const
{
    return m_minContentHeight;
}
int InventoryView::maxContentHeight() const
{
    return m_maxContentHeight;
}

int InventoryView::scroll() const
{
    return m_scroll;
}

int InventoryView::size() const
{
    return m_slotViews.size();
}

bool InventoryView::isMinimizable() const
{
    return m_isMinimizable;
}
bool InventoryView::isCloseable() const
{
    return m_isCloseable;
}
bool InventoryView::isResizeable() const
{
    return m_isResizeable;
}

void InventoryView::setMinimizable(bool newValue)
{
    m_isMinimizable = newValue;
}
void InventoryView::setCloseable(bool newValue)
{
    m_isCloseable = newValue;
}
void InventoryView::setResizeable(bool newValue)
{
    m_isResizeable = newValue;
}

void InventoryView::setContentHeight(int newHeight)
{
    m_contentHeight = newHeight;
}
void InventoryView::setContentHeightToMax()
{
    m_contentHeight = m_maxContentHeight;
}

void InventoryView::drawContent(PlayerUi& playerUi, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int numberOfSlots = m_slotViews.size();
    for(int i = 0; i < numberOfSlots; ++i)
    {
        m_slotViews[i].draw(renderTarget, renderStates, m_parentInventory->slotContentRequirement(i));
    }
}
void InventoryView::update()
{
    m_contentHeight = std::min(std::max(m_contentHeight, m_minContentHeight), m_maxContentHeight);
    m_scroll = std::min(std::max(m_scroll, 0), m_maxContentHeight - m_contentHeight);
}

void InventoryView::updateMaxContentHeight()
{
    constexpr int padding = 4;
    for(const auto& slot : m_slotViews)
    {
        int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        m_maxContentHeight = std::max(m_maxContentHeight, slotBottom + padding);
    }
}
