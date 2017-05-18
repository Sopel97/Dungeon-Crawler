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

    EventResult dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos) override;

	static constexpr int playerUiPanelWidth() { return m_playerUiPanelWidth; }
protected:

    static const int m_playerUiPanelWidth;

    WindowSpaceManager& m_windowSpaceManager;
    Player& m_player;
	std::vector<PanelWindow*> m_windows;
    PanelWindow* m_focusedWindow;

	void updateWindowPositions();
};

#endif // PLAYERUI_H
