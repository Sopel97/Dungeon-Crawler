#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

#include "WindowSpaceUser.h"

#include "../LibS/Shapes.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Game;

class PlayerUi : public WindowSpaceUser
{
public:
    // TODO: move drawing of the basic window shape to somewhere where it can be used by FreeWindow too
    class PanelWindow : public WindowSpaceManager::Window
    {
    public:
        PanelWindow(const std::string& name) :
            Window(ls::Rectangle2I::withSize(ls::Vec2I(0, 0), m_playerUiPanelWidth, 0), name)
        {

        }
    };

    PlayerUi(Player& player, const WindowSpaceManager::WindowFullLocalization& loc);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const PanelWindow& wnd); //later will somehow draw buttons.

    void setContentViewOfWindow(PanelWindow& wnd);

    void closeWindow(PanelWindow* window);
    void openWindow(PanelWindow* wnd); //does not take ownership

	void onWindowUpdated(PanelWindow& window);

	static constexpr int playerUiPanelWidth() { return m_playerUiPanelWidth; }
protected:

    static const int m_playerUiPanelWidth;
    static const int m_windowTopBarHeight;
    static const int m_windowHeaderHeight;
    static const int m_windowLeftBarWidth;
    static const int m_windowBottomBarHeight;
    static const int m_windowRightBarWidth;
    static const int m_windowScrollBarWidth;
    static const ls::Vec2I m_windowHighTopLeftCornerSize;
    static const ls::Vec2I m_windowLowTopLeftCornerSize;
    static const ls::Vec2I m_windowHighTopRightCornerSize;
    static const ls::Vec2I m_windowLowTopRightCornerSize;
    static const ls::Vec2I m_windowBottomLeftCornerSize;
    static const ls::Vec2I m_windowBottomRightCornerSize;
    static const ls::Vec2I m_windowButtonSize;
    static const ls::Vec2I m_windowScrollSliderSize;

    static const ls::Vec2I m_windowTopBarSpritePosition;
    static const ls::Vec2I m_windowHeaderSpritePosition;
    static const ls::Vec2I m_windowLeftBarSpritePosition;
    static const ls::Vec2I m_windowRightBarSpritePosition;
    static const ls::Vec2I m_windowBottomBarSpritePosition;
    static const ls::Vec2I m_windowHighTopLeftCornerSpritePosition;
    static const ls::Vec2I m_windowLowTopLeftCornerSpritePosition;
    static const ls::Vec2I m_windowHighTopRightCornerSpritePosition;
    static const ls::Vec2I m_windowLowTopRightCornerSpritePosition;
    static const ls::Vec2I m_windowBottomLeftCornerSpritePosition;
    static const ls::Vec2I m_windowBottomRightCornerSpritePosition;
    static const ls::Vec2I m_windowScrollBarSpritePosition;
    static const ls::Vec2I m_windowScrollBarUpButtonSpritePosition;
    static const ls::Vec2I m_windowScrollBarDownButtonSpritePosition;
    static const ls::Vec2I m_windowScrollBarSliderSpritePosition;

    Player& m_player;
	std::vector<PanelWindow*> m_windows;

    void drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const PanelWindow& wnd, const ls::Rectangle2I& windowRect, const ls::Rectangle2I& contentRect);

	void updateWindowPositions();
};

#endif // PLAYERUI_H
