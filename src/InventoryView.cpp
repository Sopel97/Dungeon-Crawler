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
    m_offsetFromTop(0),
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
    m_offsetFromTop(0),
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
    m_offsetFromTop(0),
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

InventoryView::InventoryView(const InventoryView& other) :
    m_parentInventory(other.m_parentInventory),
    m_slotViews(other.m_slotViews),
    m_offsetFromTop(other.m_offsetFromTop),
    m_scroll(other.m_scroll),
    m_contentHeight(other.m_contentHeight),
    m_minContentHeight(other.m_minContentHeight),
    m_maxContentHeight(other.m_maxContentHeight),
    m_isMinimizable(other.m_isMinimizable),
    m_isCloseable(other.m_isCloseable),
    m_isResizeable(other.m_isResizeable)
{

}
InventoryView::InventoryView(InventoryView&& other) :
    m_parentInventory(std::move(other.m_parentInventory)),
    m_slotViews(std::move(other.m_slotViews)),
    m_offsetFromTop(std::move(other.m_offsetFromTop)),
    m_scroll(std::move(other.m_scroll)),
    m_contentHeight(std::move(other.m_contentHeight)),
    m_minContentHeight(std::move(other.m_minContentHeight)),
    m_maxContentHeight(std::move(other.m_maxContentHeight)),
    m_isMinimizable(std::move(other.m_isMinimizable)),
    m_isCloseable(std::move(other.m_isCloseable)),
    m_isResizeable(std::move(other.m_isResizeable))
{

}

InventoryView& InventoryView::operator =(const InventoryView& other)
{
    m_parentInventory = other.m_parentInventory;
    m_slotViews = other.m_slotViews;
    m_offsetFromTop = other.m_offsetFromTop;
    m_scroll = other.m_scroll;
    m_contentHeight = other.m_contentHeight;
    m_minContentHeight = other.m_minContentHeight;
    m_maxContentHeight = other.m_maxContentHeight;
    m_isMinimizable = other.m_isMinimizable;
    m_isCloseable = other.m_isCloseable;
    m_isResizeable = other.m_isResizeable;

    return *this;
}
InventoryView& InventoryView::operator =(InventoryView&& other)
{
    m_parentInventory = std::move(other.m_parentInventory);
    m_slotViews = std::move(other.m_slotViews);
    m_offsetFromTop = std::move(other.m_offsetFromTop);
    m_scroll = std::move(other.m_scroll);
    m_contentHeight = std::move(other.m_contentHeight);
    m_minContentHeight = std::move(other.m_minContentHeight);
    m_maxContentHeight = std::move(other.m_maxContentHeight);
    m_isMinimizable = std::move(other.m_isMinimizable);
    m_isCloseable = std::move(other.m_isCloseable);
    m_isResizeable = std::move(other.m_isResizeable);

    return *this;
}


void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
    updateMaxContentHeight();
}

void InventoryView::setOffsetFromTop(int newOffset)
{
    m_offsetFromTop = newOffset;
}

Inventory const* InventoryView::parentInventory() const
{
    return m_parentInventory;
}
int InventoryView::offsetFromTop() const
{
    return m_offsetFromTop;
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

void InventoryView::draw(PlayerUi& playerUi, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    playerUi.drawWindowOnPanel(renderTarget, renderStates, *this);

    playerUi.setContentViewOfPanelWindow(*this);

    for(auto& slot : m_slotViews) slot.draw(renderTarget, renderStates);
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
