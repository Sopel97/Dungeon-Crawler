#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

#include "window/WindowContent.h"
#include "window/InternalWindow.h"

#include "../LibS/Shapes.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Game;

class PlayerUi : public WindowContent
{
public:
    // TODO: move drawing of the basic window shape to somewhere where it can be used by FreeWindow too
    class PanelWindow : public InternalWindow
    {
    public:
        PanelWindow(WindowSpaceManager& wsm, const std::string& name, const WindowParams& params) :
            InternalWindow(wsm, ls::Rectangle2I::withSize(ls::Vec2I(0, 0), m_playerUiPanelWidth, 0), name, params)
        {

        }
    };

    PlayerUi(WindowSpaceManager& wsm, Player& player, InternalWindow& wnd);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    void closeWindow(PanelWindow* window);
    void openWindow(PanelWindow* wnd); //does not take ownership

	void onWindowUpdated(PanelWindow& window);

    // TODO: elegant dispatcher and storing a focused window (separate from Scene's one)
    EventResult onTextEntered(sf::Event::TextEvent& event, EventContext context) override;
    EventResult onKeyPressed(sf::Event::KeyEvent& event, EventContext context) override;
    EventResult onKeyReleased(sf::Event::KeyEvent& event, EventContext context) override;
    EventResult onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context) override;
    EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context) override;
    EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context) override;
    EventResult onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context) override;

	static constexpr int playerUiPanelWidth() { return m_playerUiPanelWidth; }
protected:

    static const int m_playerUiPanelWidth;

    WindowSpaceManager& m_windowSpaceManager;
    Player& m_player;
	std::vector<PanelWindow*> m_windows;

	void updateWindowPositions();
};

#endif // PLAYERUI_H
