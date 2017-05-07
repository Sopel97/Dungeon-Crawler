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

class WindowSpaceUser;

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

   /* class WindowRegion
    {
    private:
        ls::Rectangle2I m_rect;
        std::string m_name;
        WindowSpaceUser* m_spaceUser;

    public:
        WindowRegion(const ls::Rectangle2I& rect, const std::string& name);

        const ls::Rectangle2I& rect() const;
        const std::string& name() const;

        bool hasEventHandler() const;
        SfmlEventHandler& eventHandler();

        void setRect(const ls::Rectangle2I& newRect);
        void setUser(WindowSpaceUser& newUser);

        ls::Vec2I localCoords(const ls::Vec2I& windowCoords) const;

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    };*/

    class Window
    {
    private:
        ls::Rectangle2I m_windowRect;
        std::string m_name;
        Window* m_parent;
        WindowSpaceUser* m_spaceUser;

    public:
        Window(const ls::Rectangle2I& windowRect, const std::string& name);

        virtual ~Window();

        virtual ls::Rectangle2I windowRect() const;
        virtual ls::Rectangle2I contentRect() const;
        virtual const std::string& title() const;

        virtual void setWindowRect(const ls::Rectangle2I& newRect);
        virtual void setWindowPosition(const ls::Vec2I& newPosition);
        virtual void setWindowWidth(int newWidth);
        virtual void setWindowHeight(int newHeight);
        virtual void setWindowSize(const ls::Vec2I& newSize);
        virtual void setContentSize(const ls::Vec2I& newSize);
        virtual void setContentWidth(int newWidth);
        virtual void setContentHeight(int newHeight);

        virtual ls::Vec2I minWindowSize() const;
        virtual bool hasMaxWindowSize() const;
        virtual ls::Vec2I maxWindowSize() const;
        virtual ls::Vec2I minContentSize() const;
        virtual bool hasMaxContentSize() const;
        virtual ls::Vec2I maxContentSize() const;

        virtual int verticalScroll() const;

        virtual bool isMinimizable() const;
        virtual bool isCloseable() const;
        virtual bool isResizeable() const;
        virtual bool isMovable() const;

        virtual bool hasHeader() const;
        virtual bool hasScrollBar() const;

        virtual bool hasEventHandler() const;
        virtual SfmlEventHandler& eventHandler();

        virtual void setParent(Window& parent);
        virtual bool hasParent() const;
        virtual void removeParent();

        virtual void setUser(WindowSpaceUser& newUser);
        virtual void removeUser();
        virtual WindowSpaceUser* user();

        virtual ls::Vec2I localWindowCoords(const ls::Vec2I& globalCoords) const;
        virtual ls::Vec2I localContentCoords(const ls::Vec2I& globalCoords) const;

        virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    private:
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
        std::optional<SubdivisionParams> m_params;
    public:
        BackgroundWindow(const ls::Rectangle2I& rect, const std::string& name) :
            Window(rect, name),
            m_params(std::nullopt)
        {
        }

        virtual ls::Rectangle2I contentRect() const
        {
            return windowRect();
        }

        const std::optional<SubdivisionParams>& params() const
        {
            return m_params;
        }
        std::optional<SubdivisionParams>& params()
        {
            return m_params;
        }

        void setParams(const SubdivisionParams& params)
        {
            m_params = params;
        }
    };

    using BackgroundWindowsStorage = ls::BinaryTree<BackgroundWindow>;
    using BackgroundWindowHandle = typename BackgroundWindowsStorage::Iterator;
    using ConstBackgroundWindowHandle = typename BackgroundWindowsStorage::ConstIterator;

    class FreeWindowParams
    {
    private:
        ls::Vec2I m_defaultSize;
        ls::Vec2I m_minSize;
        std::optional<ls::Vec2I> m_maxSize;
        std::optional<const Window*> m_parentWindow;
        bool m_showHeader;

    public:
        FreeWindowParams(const ls::Vec2I& defaultSize);

        FreeWindowParams(const FreeWindowParams& other) = default;
        FreeWindowParams(FreeWindowParams&& other) = default;
        FreeWindowParams& operator=(const FreeWindowParams& other) = default;
        FreeWindowParams& operator=(FreeWindowParams&& other) = default;

        FreeWindowParams& withMinSize(const ls::Vec2I& minSize);
        FreeWindowParams& withMaxSize(const ls::Vec2I& maxSize);
        FreeWindowParams& withParentWindow(const Window& window);
        FreeWindowParams& withHeader();

        const ls::Vec2I& defaultSize() const;
        const ls::Vec2I& minSize() const;
        bool hasMaxSize() const;
        const ls::Vec2I& maxSize() const;
        bool hasParentWindow() const;
        const Window& parentWindow() const;

        bool hasHeader() const;
    };

    class FreeWindow : public Window
    {
    private:
        FreeWindowParams m_params;
    public:
        FreeWindow(const ls::Rectangle2I& rect, const std::string& name, const FreeWindowParams& params) :
            Window(rect, name),
            m_params(params)
        {
        }

        const FreeWindowParams& params() const
        {
            return m_params;
        }
        FreeWindowParams& params()
        {
            return m_params;
        }

        virtual bool isCloseable() const
        {
            return true;
        }
        virtual bool isMovable() const
        {
            return true;
        }

        virtual bool hasHeader() const
        {
            return m_params.hasHeader();
        }

        void setParams(const FreeWindowParams& params)
        {
            m_params = params;
        }
    };

    using FreeWindowStorage = std::list<FreeWindow>;
    using FreeWindowHandle = typename FreeWindowStorage::iterator;
    using ConstFreeWindowHandle = typename FreeWindowStorage::const_iterator;

    struct WindowFullLocalization
    {
        WindowSpaceManager* windowSpaceManager;
        Window* window;
    };

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
        FreeWindowHandle createFreeWindow(const FreeWindowParams& params, const std::string& name, const ls::Vec2I& center);

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

        WindowFullLocalization fullLocalizationOf(BackgroundWindowHandle h);

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
                const SfmlEventHandler::EventContext context{ true, ls::intersect(focused.windowRect(), mousePos) };
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
