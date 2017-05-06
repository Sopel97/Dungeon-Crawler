#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

const int InventoryWindow::m_minSensibleHeight = 46;

InventoryView::InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory) :
    WindowSpaceUser(loc),
    m_inventory(&inventory)
{
}
InventoryView::InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory, const std::vector<InventorySlotView>& slots) :
    WindowSpaceUser(loc),
    m_inventory(&inventory),
    m_slotViews(slots)
{
}
InventoryView::InventoryView(const WindowSpaceManager::WindowFullLocalization& loc, Inventory& inventory, std::vector<InventorySlotView>&& slots) :
    WindowSpaceUser(loc),
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

void InventoryView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int numberOfSlots = m_slotViews.size();
    for (int i = 0; i < numberOfSlots; ++i)
    {
        m_slotViews[i].draw(renderTarget, renderStates, m_inventory->slotContentRequirement(i));
    }
}

InventoryWindow::InventoryWindow(const std::string& name) :
    PanelWindow(name),
    m_scroll(0),
    m_minContentHeight(m_minSensibleHeight),
    m_maxContentHeight(m_minSensibleHeight),
    m_isMinimizable(false),
    m_isCloseable(true),
    m_isResizeable(false),
    m_isMovable(false),
    m_hasHeader(false),
    m_hasScrollBar(false)
{
}

ls::Vec2I InventoryWindow::minContentSize() const
{
    return ls::Vec2I(0, m_minContentHeight);
}
ls::Vec2I InventoryWindow::minWindowSize() const
{
    return ls::Vec2I(PlayerUi::playerUiPanelWidth(), 0);
}
bool InventoryWindow::hasMaxWindowSize() const
{
    return true;
}
ls::Vec2I InventoryWindow::maxWindowSize() const
{
    return ls::Vec2I(PlayerUi::playerUiPanelWidth(), m_maxContentHeight + 999); //margin for the border, is restricted by maxContentSize.y
}
bool InventoryWindow::hasMaxContentSize() const
{
    return true;
}
ls::Vec2I InventoryWindow::maxContentSize() const
{
    return ls::Vec2I(PlayerUi::playerUiPanelWidth()-8, m_maxContentHeight); //-8 should not be there, will be removed when checking constraints for windows and adjusting size will be made
}

int InventoryWindow::verticalScroll() const
{
    return m_scroll;
}

bool InventoryWindow::isMinimizable() const
{
    return m_isMinimizable;
}
bool InventoryWindow::isCloseable() const
{
    return m_isCloseable;
}
bool InventoryWindow::isResizeable() const
{
    return m_isResizeable;
}
bool InventoryWindow::isMovable() const
{
    return m_isMovable;
}

bool InventoryWindow::hasHeader() const
{
    return m_hasHeader;
}
bool InventoryWindow::hasScrollBar() const
{
    return m_hasScrollBar;
}

void InventoryWindow::setMinimizable(bool newValue)
{
    m_isMinimizable = newValue;
}
void InventoryWindow::setCloseable(bool newValue)
{
    m_isCloseable = newValue;
}
void InventoryWindow::setResizeable(bool newValue)
{
    m_isResizeable = newValue;
}
void InventoryWindow::setMovable(bool newValue)
{
    m_isMovable = newValue;
}
void InventoryWindow::setHeaderEnabled(bool newValue)
{
    m_hasHeader = newValue;
}
void InventoryWindow::setScrollBarEnabled(bool doEnable)
{
    m_hasScrollBar = doEnable;
}

void InventoryWindow::setContentSizeToMax()
{
    this->setContentSize(maxContentSize());
}
void InventoryWindow::update()
{
    m_scroll = std::min(std::max(m_scroll, 0), m_maxContentHeight - this->contentRect().height());
}

void InventoryWindow::updateMaxContentHeight(const InventoryView& view)
{
    m_maxContentHeight = 0;

    constexpr int padding = 4;
    for(const auto& slot : view.slotViews())
    {
        int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        m_maxContentHeight = std::max(m_maxContentHeight, slotBottom + padding);
    }
}

WindowSpaceManager::WindowFullLocalization InventoryWindow::fullLocalization()
{
    return { &(Root::instance().windowSpaceManager()), this };
}
