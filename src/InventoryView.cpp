#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

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

WindowSpaceManager::WindowParams InventoryWindow::defaultParams()
{
    WindowSpaceManager::WindowParams params;

    params.minWindowWidth = PlayerUi::playerUiPanelWidth();
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = m_minSensibleHeight;

    params.maxWindowWidth = PlayerUi::playerUiPanelWidth();
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = m_minSensibleHeight;

    params.isMinimizable = true;
    params.isCloseable = true;
    params.isResizeable = true;
    params.isMovable = true;
    params.isContentOnly = false;
    params.hasHeader = true;
    params.hasScrollBar = true;

    return params;
}

InventoryWindow::InventoryWindow(const std::string& name) :
    PanelWindow(name, defaultParams()),
    m_scroll(0)
{
}
InventoryWindow::InventoryWindow(const std::string& name, const WindowSpaceManager::WindowParams& params) :
    PanelWindow(name, params),
    m_scroll(0)
{
}

int InventoryWindow::verticalScroll() const
{
    return m_scroll;
}


void InventoryWindow::setContentHeightToMax()
{
    setContentHeight(maxContentHeight());
}
void InventoryWindow::update()
{
    m_scroll = std::min(std::max(m_scroll, 0), maxContentHeight() - contentRect().height());
}

void InventoryWindow::updateMaxContentHeight(const InventoryView& view)
{
    int newMaxContentHeight = 0;

    constexpr int padding = 4;
    for(const auto& slot : view.slotViews())
    {
        const int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        newMaxContentHeight = std::max(newMaxContentHeight, slotBottom + padding);
    }

    setMaxContentHeight(newMaxContentHeight);
}

WindowSpaceManager::WindowFullLocalization InventoryWindow::fullLocalization()
{
    return { &(Root::instance().windowSpaceManager()), this };
}
