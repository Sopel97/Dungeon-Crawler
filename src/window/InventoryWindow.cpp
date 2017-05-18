#include "window/InventoryWindow.h"

#include "InventoryView.h"

WindowParams InventoryWindow::defaultParams()
{
    WindowParams params;

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

InventoryWindow::InventoryWindow(WindowSpaceManager& wsm, const std::string& name) :
    PanelWindow(wsm, name, defaultParams()),
    m_scroll(0)
{
}
InventoryWindow::InventoryWindow(WindowSpaceManager& wsm, const std::string& name, const WindowParams& params) :
    PanelWindow(wsm, name, params),
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
    m_scroll = std::min(std::max(m_scroll, 0), maxContentHeight() - absoluteContentRect().height());
}

void InventoryWindow::updateMaxContentHeight(const InventoryView& view)
{
    int newMaxContentHeight = 0;

    constexpr int padding = 4;
    for (const auto& slot : view.slotViews())
    {
        const int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        newMaxContentHeight = std::max(newMaxContentHeight, slotBottom + padding);
    }

    setMaxContentHeight(newMaxContentHeight);
}
