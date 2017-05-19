#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Player.h"
#include "Root.h"
#include "window/WindowSpaceManager.h"

#include <algorithm>

using namespace ls;

const int PlayerUi::m_playerUiPanelWidth = 230;

PlayerUi::PlayerUi(WindowSpaceManager& wsm, Player& player, InternalWindow& wnd) :
	WindowContent(wnd),
    m_windowSpaceManager(wsm),
    m_player(player),
    m_focusedWindow(nullptr)
{
}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for(auto wnd : m_windows)
    {
        wnd->draw(renderTarget, renderStates);
    }
}

void PlayerUi::closeWindow(PanelWindow* window)
{
    m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
    if (window == m_focusedWindow) m_focusedWindow = nullptr;
	updateWindowPositions();
}
void PlayerUi::openWindow(PanelWindow* wnd)
{
    wnd->setParent(window());
	m_windows.push_back(wnd);

	updateWindowPositions();
}

void PlayerUi::updateWindowPositions()
{
	int currentPosition = 0;
	for (PanelWindow* window : m_windows)
	{
		window->setWindowPosition(ls::Vec2I(0, currentPosition));
		currentPosition += window->windowRect().height();
	}
}

void PlayerUi::onWindowUpdated(PanelWindow& window)
{
	updateWindowPositions();
}

SfmlEventHandler::EventResult PlayerUi::dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos)
{
    auto result = SfmlEventHandler::dispatch(event, context, mousePos);
    if (result.consumeEvent) return result;

    // TODO: instead of looping here twice make a query function
    PanelWindow* windowUnderMouse = nullptr;
    if (context.isMouseOver)
    {
        for (auto& wnd : m_windows)
        {
            if (!ls::intersect(mousePos, wnd->absoluteContentRect())) continue;

            bool isFocused = wnd == m_focusedWindow;
            EventContext currentContext = EventContext{}
                .setIsFocused(isFocused)
                .setIsMouseOver(true);
            result = wnd->dispatch(event, currentContext, mousePos);

            if (result.takeFocus) m_focusedWindow = wnd;
            if (result.consumeEvent) break;
        }
    }

    if (!result.consumeEvent)
    {
        if (m_focusedWindow != nullptr && m_focusedWindow != windowUnderMouse)
        {
            EventContext currentContext = EventContext{}
                .setIsFocused()
                .setIsMouseOver(false);
            result = m_focusedWindow->dispatch(event, currentContext, mousePos);
        }
    }
        
    if (!result.consumeEvent)
    {
        for (auto& wnd : m_windows)
        {
            if (wnd == windowUnderMouse || wnd == m_focusedWindow) continue;
            EventContext currentContext = EventContext{}
                .setIsFocused(false)
                .setIsMouseOver(false);
            result = wnd->dispatch(event, currentContext, mousePos);

            if (result.takeFocus) m_focusedWindow = wnd;
            if (result.consumeEvent) break;
        }
    }

    updateWindowPositions();

    return result;
}