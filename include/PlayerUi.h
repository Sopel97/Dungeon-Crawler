#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

#include "../LibS/Shapes.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Game;

class PlayerUi
{
public:
    class Window
    {
    public:
        virtual int offsetFromTop() const { return 0; } //has specified either this or the position depending on where is stored
        virtual ls::Vec2I position() const { return ls::Vec2I{0, 0}; }
        virtual int contentWidth() const { return m_playerUiPanelWidth; }
        virtual int contentHeight() const { return 0; }

        virtual int minContentHeight() const { return 0; }
        virtual int maxContentHeight() const { return 0; }

        virtual int scroll() const { return 0; }

        virtual bool isMinimizable() const { return false; }
        virtual bool isCloseable() const { return false; }
        virtual bool isResizeable() const { return false; }

        virtual std::string title() const { return ""; }
        virtual bool hasHeader() const { return true; }
        virtual bool hasScrollBar() const { return true; }

        virtual int windowWidth() const { return this->contentWidth() + m_windowLeftBarWidth + m_windowRightBarWidth; }
        virtual int windowHeight() const 
        {
            const int& topBarHeight = this->hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
            return this->contentHeight() + m_windowBottomBarHeight + topBarHeight;
        }

        virtual void draw(PlayerUi& playerUi, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;
    };

    PlayerUi(Game& game, Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawWindowOnPanel(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window); //later will somehow draw buttons.
    void drawPopupWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window); //later will somehow draw buttons.

    void setContentViewOfPanelWindow(Window& window);
    void setContentViewOfPopupWindow(Window& window);

    void closeWindow(Window* window);
    void openPanelWindow(Window* window); //does not take ownership
    void openPopupWindow(Window* window); //does not take ownership
    //TODO: move window position setting from inventory system to here

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

    Game& m_game;
    Player& m_player;

    std::vector<Window*> m_ownedWindows;
    std::vector<Window*> m_panelWindows;
    std::vector<Window*> m_popupWindows;

    void drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window, const ls::RectangleI& rect);
};

#endif // PLAYERUI_H
