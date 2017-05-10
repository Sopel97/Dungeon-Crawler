#ifndef WINDOWSPACEMANAGER_H
#define WINDOWSPACEMANAGER_H

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <map>
#include <optional>
#include <utility>
#include <list>

#include "SfmlEventHandler.h"

class WindowContent;

class WindowSpaceManager
{
public:
    class Scene;
    class SubdivisionParams;

    class AspectRatio
    {
    private:
        float m_ratio;
    public:
        AspectRatio(float r);
        AspectRatio(int h, int v);

        ls::Rectangle2I fitRectCentered(const ls::Rectangle2I& space) const;

        float ratio() const;
        void setRatio(float r);
        void setRatio(int h, int v);
    };

    class SubdivisionParams
    {
    public:
        enum class Orientation
        {
            Horizontal,
            Vertical
        };
        enum class Subject
        {
            First,
            Second
        };

    private:
        enum class AmountType
        {
            Pixels,
            Ratio
        };
        union Amount
        {
            int pixels;
            float ratio;
        };

        Amount m_amount;
        AmountType m_amountType;
        Subject m_subject;
        Orientation m_orientation;
        std::optional<AspectRatio> m_aspectRatioFirst;
        std::optional<AspectRatio> m_aspectRatioSecond;

    public:

        static SubdivisionParams withPixels(Orientation orientation, Subject subject, int pixels);
        static SubdivisionParams withRatio(Orientation orientation, Subject subject, float ratio);

        SubdivisionParams& withAspectRatio(AspectRatio ratio, Subject subject);

        SubdivisionParams(const SubdivisionParams& other) = default;
        SubdivisionParams(SubdivisionParams&& other) = default;

        SubdivisionParams& operator=(const SubdivisionParams& other) = default;
        SubdivisionParams& operator=(SubdivisionParams&& other) = default;

        void setPixels(int newPixels);
        void setRatio(float newRatio);

        std::pair<ls::Rectangle2I, ls::Rectangle2I> calculateSubRects(const ls::Rectangle2I& rect) const;

        void moveByPixels(const ls::Rectangle2I& rect, int deltaPixels);
        void moveByRatio(const ls::Rectangle2I& rect, float deltaRatio);

        int asPixels(const ls::Rectangle2I& rect) const;
        float asRatio(const ls::Rectangle2I& rect) const;

    private:
        SubdivisionParams(Orientation orientation, Subject subject, int pixels);
        SubdivisionParams(Orientation orientation, Subject subject, float ratio);
    };
    
    struct WindowParams
    {
        int minWindowWidth;
        int minWindowHeight;
        int minContentWidth;
        int minContentHeight;

        std::optional<int> maxWindowWidth;
        std::optional<int> maxWindowHeight;
        std::optional<int> maxContentWidth;
        std::optional<int> maxContentHeight;

        bool isMinimizable;
        bool isCloseable;
        bool isResizeable;
        bool isMovable;
        bool isContentOnly;
        bool hasHeader;
        bool hasScrollBar;
    };

    class Window
    {
    private:
        ls::Rectangle2I m_windowRect;
        WindowParams m_params;
        std::string m_name;
        WindowSpaceManager* m_wsm;
        Window* m_parent;
        WindowContent* m_content;

    public:
        static WindowParams defaultParams();

        Window(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name);
        Window(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name, const WindowParams& params);

        Window(const Window&) = delete;
        Window(Window&&) = default;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = default;

        virtual ~Window();

        const ls::Rectangle2I& windowRect() const;
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

        void setMinimizable(bool val);
        void setCloseable(bool val);
        void setResizeable(bool val);
        void setMovable(bool val);
        void setContentOnly(bool val);
        void setHeaderEnabled(bool val);
        void setScrollBarEnabled(bool val);

        bool hasEventHandler() const;
        SfmlEventHandler& eventHandler();

        void setParent(Window& parent);
        bool hasParent() const;
        void removeParent();

        void setContent(WindowContent& newUser);
        void removeContent();

        WindowSpaceManager& windowSpaceManager();
        const WindowSpaceManager& windowSpaceManager() const;

        ls::Vec2I localWindowCoords(const ls::Vec2I& globalCoords) const;
        ls::Vec2I localContentCoords(const ls::Vec2I& globalCoords) const;

        virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    private:

        void drawSkeleton(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

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

    // technically not a window
    class BackgroundWindow : public Window
    {
    private:
        std::optional<SubdivisionParams> m_subdivisionParams;
    public:

        static WindowParams defaultParams();

        BackgroundWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name) :
            Window(wsm, rect, name, defaultParams()),
            m_subdivisionParams(std::nullopt)
        {
        }
        BackgroundWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name, BackgroundWindow& parent) :
            Window(wsm, rect, name, defaultParams()),
            m_subdivisionParams(std::nullopt)
        {
            setParent(parent);
        }

        const std::optional<SubdivisionParams>& params() const
        {
            return m_subdivisionParams;
        }
        std::optional<SubdivisionParams>& params()
        {
            return m_subdivisionParams;
        }

        void setParams(const SubdivisionParams& params)
        {
            m_subdivisionParams = params;
        }
    };

    using BackgroundWindowsStorage = ls::BinaryTree<BackgroundWindow>;
    using BackgroundWindowHandle = typename BackgroundWindowsStorage::Iterator;
    using ConstBackgroundWindowHandle = typename BackgroundWindowsStorage::ConstIterator;

    class FreeWindow : public Window
    {
    public:

        static WindowParams defaultParams();

        FreeWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name) :
            Window(wsm, rect, name)
        {
        }
    };

    using FreeWindowStorage = std::list<FreeWindow>;
    using FreeWindowHandle = typename FreeWindowStorage::iterator;
    using ConstFreeWindowHandle = typename FreeWindowStorage::const_iterator;

    class Scene
    {
    private:
        WindowSpaceManager* m_windowSpaceManager;
        BackgroundWindowsStorage m_backgroundWindows;
        //TODO: currently free windows can only be created, make it so they are also drawn and participate in event dispatch. 
        // Also do more functions on free windows, rename current ones that work only on background windows so they are not mistook as generic
        FreeWindowStorage m_freeWindows;
        std::string m_name;
        std::vector<BackgroundWindowHandle> m_topmostRegions;
        BackgroundWindowHandle m_focusedRegionHandle;

    public:
        Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name);

        Scene(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = default;

        BackgroundWindow& window(BackgroundWindowHandle h);
        const BackgroundWindow& window(ConstBackgroundWindowHandle h) const;

        bool isSubdivided(ConstBackgroundWindowHandle h) const;

        SubdivisionParams& subdivisionParams(BackgroundWindowHandle h);
        const SubdivisionParams& subdivisionParams(ConstBackgroundWindowHandle h) const;

        std::pair<BackgroundWindowHandle, BackgroundWindowHandle> subdivide(BackgroundWindowHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond);
        FreeWindowHandle createFreeWindow(const std::string& name, const ls::Vec2I& center, const ls::Vec2I& size);

        BackgroundWindowHandle firstChild(BackgroundWindowHandle h);
        ConstBackgroundWindowHandle firstChild(ConstBackgroundWindowHandle h) const;
        BackgroundWindowHandle secondChild(BackgroundWindowHandle h);
        ConstBackgroundWindowHandle secondChild(ConstBackgroundWindowHandle h) const;
        BackgroundWindowHandle parent(BackgroundWindowHandle h);
        ConstBackgroundWindowHandle parent(ConstBackgroundWindowHandle h) const;
        const std::string& name() const;
        bool hasChildren(ConstBackgroundWindowHandle h) const;
        bool hasParent(ConstBackgroundWindowHandle h) const;

        BackgroundWindowHandle rootHandle();
        ConstBackgroundWindowHandle rootHandle() const;

        BackgroundWindowHandle findBackgroundWindow(const std::string& title);
        ConstBackgroundWindowHandle findBackgroundWindow(const std::string& title) const;

        void update(const ls::Rectangle2I& rect);

        BackgroundWindowHandle queryRegion(const ls::Vec2I& pos);
        ConstBackgroundWindowHandle queryRegion(const ls::Vec2I& pos) const;

        bool tryDispatchEvent(sf::Event& event, const ls::Vec2I& mousePos);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    private:
        void setSubdivisionParams(BackgroundWindowHandle h, const SubdivisionParams& params);
        void update(BackgroundWindowHandle h, const ls::Rectangle2I& rect);
        
        template <class EventType>
        void dispatchEvent(EventType& event, SfmlEventHandler::EventResult(SfmlEventHandler::*handler)(EventType&, SfmlEventHandler::EventContext), const ls::Vec2I& mousePos)
        {
            BackgroundWindowHandle focusedRegionHandle = m_focusedRegionHandle; //stored because can be changed midway
            Window& focused = window(focusedRegionHandle);
            if (focused.hasEventHandler())
            {
                const SfmlEventHandler::EventContext context{ true, ls::intersect(focused.absoluteWindowRect(), mousePos) };
                const SfmlEventHandler::EventResult result = (focused.eventHandler().*handler)(event, context);
                if (result.consumeEvent)
                {
                    return;
                }
            }

            // other regions

            BackgroundWindowHandle mouseOverRegionHandle = queryRegion(mousePos);
            if (mouseOverRegionHandle.isValid() && mouseOverRegionHandle != m_focusedRegionHandle)
            {
                Window& mouseOverRegion = window(mouseOverRegionHandle);
                if (mouseOverRegion.hasEventHandler())
                {
                    const SfmlEventHandler::EventContext context{ false, true };
                    const SfmlEventHandler::EventResult result = (mouseOverRegion.eventHandler().*handler)(event, context);
                    if (result.takeFocus)
                    {
                        m_focusedRegionHandle = mouseOverRegionHandle;
                    }
                    if (result.consumeEvent)
                    {
                        return;
                    }
                }
            }

            for (BackgroundWindowHandle h : m_topmostRegions)
            {
                if (h == focusedRegionHandle || h == mouseOverRegionHandle) continue;

                Window& region = window(h);
                if (region.hasEventHandler())
                {
                    const SfmlEventHandler::EventContext context{ false, false };
                    const SfmlEventHandler::EventResult result = (region.eventHandler().*handler)(event, context);
                    if (result.takeFocus)
                    {
                        m_focusedRegionHandle = h;
                    }
                    if (result.consumeEvent)
                    {
                        return;
                    }
                }
            }
        }
    };

    WindowSpaceManager(sf::RenderWindow& window);
    WindowSpaceManager(const WindowSpaceManager&) = delete;
    WindowSpaceManager(WindowSpaceManager&& other) = delete;
    WindowSpaceManager& operator=(const WindowSpaceManager&) = delete;
    WindowSpaceManager& operator=(WindowSpaceManager&& other) = delete;

    Scene& createScene(const std::string& name);

    Scene& scene(const std::string& name);
    const Scene& scene(const std::string& name) const;

    void changeScene(const std::string& name);

    ls::Rectangle2I rect() const;

    void onWindowResized();

    sf::View getRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const;
    sf::View getWindowView(const Window& window, const ls::Rectangle2F& worldRect) const;
    void setRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const;
    void setWindowView(const Window& window, const ls::Rectangle2F& worldRect) const;
    void setView(const sf::View& view);
    void setDefaultView();

    bool tryDispatchEvent(sf::Event& event);

    void drawCurrentScene(sf::RenderStates& renderStates);

    sf::RenderWindow& window();

private:
    sf::RenderWindow& m_window;
    std::map<std::string, Scene> m_scenes;
    Scene* m_currentScene;

    sf::FloatRect viewportConvertToRatio(const ls::Rectangle2I& rect) const;
};


#endif // WINDOWSPACEMANAGER_H
