#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../Libs/Util.h"

#include "Player.h"
#include "Root.h"
#include "window/WindowSpaceManager.h"

#include <algorithm>

using namespace ls;

const int PlayerUi::m_playerUiPanelWidth = 230;

PlayerUi::PlayerUi(WindowSpaceManager& wsm, Player& player) :
    m_windowSpaceManager(wsm),
    m_player(player),
    m_focusedWindow(nullptr),
    m_tileDescriptionRenderer(wsm)
{
}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    for (auto& wnd : m_windows)
    {
        wnd->draw(renderTarget, renderStates);
    }
}

void PlayerUi::closeWindow(PanelWindow* window)
{
    window->close();
    removeClosingWindows();
}
void PlayerUi::openWindow(std::unique_ptr<PanelWindow>&& wnd)
{
    if (hasWindow()) wnd->setParent(window());
    m_windows.emplace_back(std::move(wnd));

    updateWindowPositions();
}
bool PlayerUi::isOpened(PanelWindow* wnd) const
{
    return std::find_if(m_windows.begin(), m_windows.end(), [wnd](const std::unique_ptr<PanelWindow>& w)->bool {return w.get() == wnd; }) != m_windows.end();
}

void PlayerUi::removeClosingWindows()
{
    m_windows.erase(std::remove_if(m_windows.begin(), m_windows.end(), [](const std::unique_ptr<PanelWindow>& wnd)->bool {return wnd->isClosing(); }), m_windows.end());
    if (std::none_of(m_windows.begin(), m_windows.end(), [this](const std::unique_ptr<PanelWindow>& wnd)->bool {return wnd.get() == this->m_focusedWindow; }))
    {
        m_focusedWindow = nullptr;
    }
    updateWindowPositions();
}
void PlayerUi::updateWindowPositions()
{
    int currentPosition = 0;
    for (auto& window : m_windows)
    {
        window->setWindowPosition(ls::Vec2I(0, currentPosition));
        currentPosition += window->windowRect().height();
    }
}

void PlayerUi::onWindowUpdated(PanelWindow& window)
{
    updateWindowPositions();
}
void PlayerUi::onAttached(InternalWindow& wnd)
{
    WindowContent::onAttached(wnd);

    for (auto& w : m_windows)
    {
        w->setParent(wnd);
    }

    updateWindowPositions();
}
void PlayerUi::onDetached(InternalWindow& wnd)
{
    WindowContent::onDetached(wnd);

    for (auto& w : m_windows)
    {
        w->removeParent();
    }

    updateWindowPositions();
}
void PlayerUi::onDetachedAndWindowClosing(InternalWindow& wnd)
{
    onDetached(wnd);
}

SfmlEventHandler::EventResult PlayerUi::dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos)
{
    ls::OnScopeExit guard([this]() {
        removeClosingWindows();
    });

    auto result = SfmlEventHandler::dispatch(event, context, mousePos);
    if (result.consumeEvent) return result;

    // TODO: instead of looping here twice make a query function
    PanelWindow* windowUnderMouse = nullptr;
    if (context.isMouseOver)
    {
        for (auto& wnd : m_windows)
        {
            if (!ls::intersect(mousePos, wnd->absoluteContentRect())) continue;

            bool isFocused = wnd.get() == m_focusedWindow;
            EventContext currentContext = EventContext{}
                .setIsFocused(isFocused)
                .setIsMouseOver(true);
            result = wnd->dispatch(event, currentContext, mousePos);

            if (result.takeFocus) m_focusedWindow = wnd.get();
            if (result.consumeEvent) return result;
        }
    }

    if (m_focusedWindow != nullptr && m_focusedWindow != windowUnderMouse)
    {
        EventContext currentContext = EventContext{}
            .setIsFocused()
            .setIsMouseOver(false);
        result = m_focusedWindow->dispatch(event, currentContext, mousePos);

        if (result.consumeEvent) return result;
    }

    for (auto& wnd : m_windows)
    {
        if (wnd.get() == windowUnderMouse || wnd.get() == m_focusedWindow) continue;
        EventContext currentContext = EventContext{}
            .setIsFocused(false)
            .setIsMouseOver(false);
        result = wnd->dispatch(event, currentContext, mousePos);

        if (result.takeFocus) m_focusedWindow = wnd.get();
        if (result.consumeEvent) return result;
    }

    return result;
}
void PlayerUi::showTileDescription(const TileDescription& description)
{
    m_tileDescriptionRenderer.setDescription(description);
    displayTileDescriptionWindow();
}
void PlayerUi::showTileDescription(TileDescription&& description)
{
    m_tileDescriptionRenderer.setDescription(std::move(description));
    displayTileDescriptionWindow();
}
void PlayerUi::displayTileDescriptionWindow()
{
    const ls::Rectangle2I contentRect = m_tileDescriptionRenderer.requiredContentRect(m_windowSpaceManager.rect());

    Scene& scene = m_windowSpaceManager.currentScene();
    Scene::FreeWindowHandle h = scene.findFreeWindow("TileDescription");
    if (!scene.isValid(h))
    {
        WindowParams params = m_tileDescriptionRenderer.requiredWindowParams();
        auto wnd = InternalWindow::createWithContentRect(m_windowSpaceManager, contentRect, "TileDescription", params);
        wnd->attachContent(m_tileDescriptionRenderer);
        scene.addFreeWindow(std::move(wnd));
    }
    else
    {
        InternalWindow& wnd = **h;
        wnd.setContentRect(contentRect);
        scene.setWindowFocus(h);
    }
}