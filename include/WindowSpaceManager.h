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
    class WindowRegion;
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

    class WindowRegion
    {
    private:
        ls::Rectangle2I m_rect;
        std::string m_name;
        Scene* m_parentScene;
        WindowSpaceUser* m_spaceUser;

    public:
        WindowRegion(const ls::Rectangle2I& rect, const std::string& name, Scene& parentScene);

        const ls::Rectangle2I& rect() const;
        const std::string& name() const;
        const Scene& parentScene() const;
        Scene& parentScene();

        bool hasEventHandler() const;
        SfmlEventHandler& eventHandler();

        void setRect(const ls::Rectangle2I& newRect);
        void setUser(WindowSpaceUser& newUser);

        ls::Vec2I localCoords(const ls::Vec2I& windowCoords) const;

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    };

    template <class ParamsT>
    class Window
    {
    public:
        using ParamsType = ParamsT;
    private:
        WindowRegion m_windowRegion;
        ParamsType m_params;
    public:
        Window(const WindowRegion& windowRegion, const ParamsType& params) :
            m_windowRegion(windowRegion),
            m_params(params)
        {

        }

        WindowRegion& windowRegion()
        {
            return m_windowRegion;
        }
        const WindowRegion& windowRegion() const
        {
            return m_windowRegion;
        }
        const ParamsType& params() const
        {
            return m_params;
        }
        ParamsType& params()
        {
            return m_params;
        }

        void setParams(const ParamsType& params)
        {
            m_params = params;
        }
    };

    using BackgroundWindow = Window<std::optional<SubdivisionParams>>;
    using BackgroundWindowsStorage = ls::BinaryTree<BackgroundWindow>;
    using BackgroundWindowHandle = typename BackgroundWindowsStorage::Iterator;
    using ConstBackgroundWindowHandle = typename BackgroundWindowsStorage::ConstIterator;

    class FreeWindowParams
    {
    private:
        ls::Vec2I m_defaultSize;
        ls::Vec2I m_minSize;
        std::optional<ls::Vec2I> m_maxSize;
        std::optional<ConstBackgroundWindowHandle> m_regionRestriction;

    public:
        FreeWindowParams(const ls::Vec2I& defaultSize);

        FreeWindowParams(const FreeWindowParams& other) = default;
        FreeWindowParams(FreeWindowParams&& other) = default;
        FreeWindowParams& operator=(const FreeWindowParams& other) = default;
        FreeWindowParams& operator=(FreeWindowParams&& other) = default;

        FreeWindowParams& withMinSize(const ls::Vec2I& minSize);
        FreeWindowParams& withMaxSize(const ls::Vec2I& maxSize);
        FreeWindowParams& withRegionRestriction(ConstBackgroundWindowHandle h);

        const ls::Vec2I& defaultSize() const;
        const ls::Vec2I& minSize() const;
        bool hasMaxSize() const;
        const ls::Vec2I& maxSize() const;
        bool hasRegionRestriction() const;
        const ConstBackgroundWindowHandle& regionRestriction() const;

    };

    using FreeWindow = Window<FreeWindowParams>;
    using FreeWindowStorage = std::vector<FreeWindow>;
    using FreeWindowHandle = typename FreeWindowStorage::iterator;
    using ConstFreeWindowHandle = typename FreeWindowStorage::const_iterator;

    struct WindowRegionFullLocalization
    {
        WindowSpaceManager* windowSpaceManager;
        Scene* scene;
        WindowRegion* windowRegion;
    };

    class Scene
    {
    private:
        WindowSpaceManager* m_windowSpaceManager;
        BackgroundWindowsStorage m_backgroundWindows;
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

        WindowRegion& windowRegion(BackgroundWindowHandle h);
        const WindowRegion& windowRegion(ConstBackgroundWindowHandle h) const;

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

        BackgroundWindowHandle findBackgroundWindow(const std::string& name);
        ConstBackgroundWindowHandle findBackgroundWindow(const std::string& name) const;

        WindowRegionFullLocalization fullLocalizationOf(BackgroundWindowHandle h);

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
            WindowRegion& focused = windowRegion(focusedRegionHandle);
            if (focused.hasEventHandler())
            {
                const SfmlEventHandler::EventContext context{ true, ls::intersect(focused.rect(), mousePos) };
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
                WindowRegion& mouseOverRegion = windowRegion(mouseOverRegionHandle);
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

                WindowRegion& region = windowRegion(h);
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
    sf::View getRegionView(const WindowRegion& region, const ls::Rectangle2F& worldRect) const;
    void setRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const;
    void setRegionView(const WindowRegion& region, const ls::Rectangle2F& worldRect) const;
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
