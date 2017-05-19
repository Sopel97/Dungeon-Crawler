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

    for (auto& wnd : m_windows)
    {
        EventContext currentContext = EventContext{}
            .setIsFocused(context.isFocused && (m_focusedWindow == wnd))
            .setIsMouseOver(context.isMouseOver && ls::intersect(mousePos, wnd->absoluteContentRect()));
        result = wnd->dispatch(event, currentContext, mousePos);

        if (result.takeFocus) m_focusedWindow = wnd;
        if (result.consumeEvent) break;
    }

    updateWindowPositions();

    return result;
}