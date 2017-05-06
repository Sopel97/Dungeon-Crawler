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
    // TODO: implement it based on WindowSpaceManager::Window<>
    // move drawing of the basic window shape to somewhere where it can be used by FreeWindow too
	class PanelWindow
	{
	protected:
		int m_position;
	public:
		virtual int position() const { return m_position; } //offset from top of the panel
		virtual void setPosition(int newPosition) { m_position = newPosition; }
		virtual int minContentHeight() const { return 0; }
		virtual int maxContentHeight() const { return 0; }

		virtual int scroll() const { return 0; }

		virtual bool isMinimizable() const { return false; }
		virtual bool isCloseable() const { return false; }
		virtual bool isResizeable() const { return false; }

		virtual std::string title() const { return ""; }
		virtual bool hasHeader() const { return true; }
		virtual bool hasScrollBar() const { return true; }

		virtual int headerHeight() const 
		{
			return this->hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
		}

		virtual int contentWidth() const { return m_playerUiPanelWidth - m_windowLeftBarWidth - m_windowRightBarWidth; }
		virtual int contentHeight() const { return 0; }
		virtual ls::Rectangle2I contentRect() const 
		{ 
			return ls::Rectangle2I::withSize(
				ls::Vec2I(m_windowLeftBarWidth, this->position() + this->headerHeight()),
				this->contentWidth(),
				this->contentHeight()
			);
		}

        virtual int windowWidth() const { return m_playerUiPanelWidth; }
        virtual int windowHeight() const 
        {
			return this->contentHeight() + this->headerHeight() + m_windowBottomBarHeight;
        }
		virtual ls::Rectangle2I windowRect() const
		{
			return ls::Rectangle2I::withSize(
				ls::Vec2I(0, this->position()),
				this->windowWidth(),
				this->windowHeight()
			);
		}

        virtual void drawContent(PlayerUi& playerUi, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;
    };

    PlayerUi(Player& player, const WindowSpaceManager::WindowFullLocalization& loc);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, PanelWindow& wnd); //later will somehow draw buttons.

    void setContentViewOfWindow(PanelWindow& wnd);

    void closeWindow(PanelWindow* window);
    void openWindow(PanelWindow* window); //does not take ownership

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

    void drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, PanelWindow& window, const ls::Rectangle2I& rect);

	void updateWindowPositions();
};

#endif // PLAYERUI_H
