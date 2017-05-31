#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>
#include <memory>

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
    class PanelWindow : public InternalWindow
    {
    public:
        PanelWindow(WindowSpaceManager& wsm, const std::string& name, const WindowParams& params) :
            InternalWindow(wsm, ls::Rectangle2I::withSize(ls::Vec2I(0, 0), m_playerUiPanelWidth, 0), name, params)
        {
        }

        static WindowParams defaultParams()
        {
            WindowParams params;

            params.minWindowWidth = PlayerUi::playerUiPanelWidth();
            params.minWindowHeight = 0;
            params.minContentWidth = 0;
            params.minContentHeight = 0;

            params.maxWindowWidth = PlayerUi::playerUiPanelWidth();
            params.maxWindowHeight = std::nullopt;
            params.maxContentWidth = std::nullopt;
            params.maxContentHeight = std::nullopt;

            params.isMinimizable = true;
            params.isCloseable = true;
            params.isResizeable = true;
            params.isMovable = true;
            params.isContentOnly = false;
            params.hasHeader = true;
            params.hasScrollBar = true;

            return params;
        }
    };

    PlayerUi(WindowSpaceManager& wsm, Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    void closeWindow(PanelWindow* window);
    void openWindow(std::unique_ptr<PanelWindow>&& wnd);
    bool isOpened(PanelWindow* wnd) const;

	void onWindowUpdated(PanelWindow& window);

    void onAttached(InternalWindow& wnd) override;
    void onDetached(InternalWindow& wnd) override;
    void onDetachedAndWindowClosing(InternalWindow& wnd) override;

    EventResult dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos) override;

	static constexpr int playerUiPanelWidth() { return m_playerUiPanelWidth; }
protected:

    static const int m_playerUiPanelWidth;

    WindowSpaceManager& m_windowSpaceManager;
    Player& m_player;
	std::vector<std::unique_ptr<PanelWindow>> m_windows;
    PanelWindow* m_focusedWindow;

    void removeClosingWindows();
	void updateWindowPositions();
};

#endif // PLAYERUI_H
