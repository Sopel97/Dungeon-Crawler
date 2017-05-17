#pragma once

#include "../LibS/Shapes.h"
#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "WindowParams.h"
#include "SfmlEventHandler.h"
#include "InternalWindowHeaderButton.h"

class WindowSpaceManager;
class WindowContent;

// TODO: close, minimize (and custom) buttons, window handles events that relate to those buttons and sends callbacks that can be registered
class InternalWindow : public SfmlEventHandler
{
private:
    ls::Rectangle2I m_windowRect;
    WindowParams m_params;
    std::string m_name;
    std::vector<InternalWindowHeaderButton> m_headerButtons; // TODO: consider sorting by id
    WindowSpaceManager* m_wsm;
    InternalWindow* m_parent;
    WindowContent* m_content;
    InternalWindowHeaderButton* m_pressedButton;

protected:
    bool m_isMinimized;

public:
    static WindowParams defaultParams();

    InternalWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name);
    InternalWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name, const WindowParams& params);

    InternalWindow(const InternalWindow&) = delete;
    InternalWindow(InternalWindow&&) = delete;
    InternalWindow& operator=(const InternalWindow&) = delete;
    InternalWindow& operator=(InternalWindow&&) = delete;

    virtual ~InternalWindow();

    ls::Rectangle2I windowRect() const;
    ls::Rectangle2I contentRect() const;
    ls::Rectangle2I absoluteWindowRect() const;
    ls::Rectangle2I absoluteContentRect() const;
    const std::string& title() const;

    void setWindowRect(const ls::Rectangle2I& newRect);
    void setWindowPosition(const ls::Vec2I& newPosition);
    void setWindowWidth(int newWidth);
    void setWindowHeight(int newHeight);
    void setWindowSize(const ls::Vec2I& newSize);
    void setContentSize(const ls::Vec2I& newSize);
    void setContentWidth(int newWidth);
    void setContentHeight(int newHeight);

    int minWindowWidth() const;
    int minWindowHeight() const;
    int minContentWidth() const;
    int minContentHeight() const;
    bool hasMaxWindowWidth() const;
    bool hasMaxWindowHeight() const;
    bool hasMaxContentWidth() const;
    bool hasMaxContentHeight() const;
    int maxWindowWidth() const;
    int maxWindowHeight() const;
    int maxContentWidth() const;
    int maxContentHeight() const;

    void setMinWindowWidth(int val);
    void setMinWindowHeight(int val);
    void setMinContentWidth(int val);
    void setMinContentHeight(int val);
    void setMaxWindowWidth(int val);
    void setMaxWindowHeight(int val);
    void setMaxContentWidth(int val);
    void setMaxContentHeight(int val);

    virtual int verticalScroll() const;

    bool isMinimizable() const;
    bool isCloseable() const;
    bool isResizeable() const;
    bool isMovable() const;
    bool isContentOnly() const;
    bool hasHeader() const;
    bool hasScrollBar() const;
    bool isMinimized() const;

    void setMinimizable(bool val);
    void setCloseable(bool val);
    void setResizeable(bool val);
    void setMovable(bool val);
    void setContentOnly(bool val);
    void setHeaderEnabled(bool val);
    void setScrollBarEnabled(bool val);
    void minimize();
    void maximize();

    EventResult dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos) override;

    EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context) override;
    EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context) override;

    void setParent(InternalWindow& parent);
    bool hasParent() const;
    void removeParent();

    void setContent(WindowContent& newUser);
    void removeContent();

    template <class... Args>
    void addHeaderButton(Args&&... args)
    {
        m_headerButtons.emplace_back(std::forward<Args>(args)...);
    }
    InternalWindowHeaderButton& headerButton(int id);

    WindowSpaceManager& windowSpaceManager();
    const WindowSpaceManager& windowSpaceManager() const;

    ls::Vec2I localWindowCoords(const ls::Vec2I& globalCoords) const;
    ls::Vec2I localContentCoords(const ls::Vec2I& globalCoords) const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    static int closeButtonId();
    static int minimizeButtonId();
    static int maximizeButtonId();

private:

    void drawSkeleton(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawSkeletonMinimized(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawHeaderButtons(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    std::vector<ls::Vec2I> absoluteButtonsPositions();
    InternalWindowHeaderButton* queryButton(const ls::Vec2I& pos);

    static const int m_closeButtonId;
    static const int m_minimizeButtonId;
    static const int m_maximizeButtonId;

    static const ls::Rectangle2I m_closeButtonSprite;
    static const ls::Rectangle2I m_minimizeButtonSprite;
    static const ls::Rectangle2I m_maximizeButtonSprite;

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
};

