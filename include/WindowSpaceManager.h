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


    using WindowRegionStorageNode = std::pair<WindowRegion, std::optional<SubdivisionParams>>;
    using WindowRegionStorage = ls::BinaryTree<WindowRegionStorageNode>;
    using WindowRegionHandle = typename WindowRegionStorage::Iterator;
    using ConstWindowRegionHandle = typename WindowRegionStorage::ConstIterator;

    struct WindowRegionFullLocalization
    {
        WindowSpaceManager* windowSpaceManager;
        Scene* scene;
        WindowRegionHandle regionHandle;
    };


    class Scene
    {
    private:
        WindowSpaceManager* m_windowSpaceManager;
        WindowRegionStorage m_windowRegions;
        std::string m_name;
        std::vector<WindowRegionHandle> m_topmostRegions;
        WindowRegionHandle m_focusedRegionHandle;

    public:
        Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name);

        Scene(Scene&& other);
        Scene& operator=(Scene&& other);

        WindowRegion& windowRegion(WindowRegionHandle h);
        const WindowRegion& windowRegion(ConstWindowRegionHandle h) const;

        bool isSubdivided(ConstWindowRegionHandle h) const;

        SubdivisionParams& subdivisionParams(WindowRegionHandle h);
        const SubdivisionParams& subdivisionParams(ConstWindowRegionHandle h) const;

        std::pair<WindowRegionHandle, WindowRegionHandle> subdivide(WindowRegionHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond);

        WindowRegionHandle firstChild(WindowRegionHandle h);
        ConstWindowRegionHandle firstChild(ConstWindowRegionHandle h) const;
        WindowRegionHandle secondChild(WindowRegionHandle h);
        ConstWindowRegionHandle secondChild(ConstWindowRegionHandle h) const;
        WindowRegionHandle parent(WindowRegionHandle h);
        ConstWindowRegionHandle parent(ConstWindowRegionHandle h) const;
        const std::string& name() const;
        bool hasChildren(ConstWindowRegionHandle h) const;
        bool hasParent(ConstWindowRegionHandle h) const;

        WindowRegionHandle rootHandle();
        ConstWindowRegionHandle rootHandle() const;

        WindowRegionHandle find(const std::string& name);
        ConstWindowRegionHandle find(const std::string& name) const;

        WindowRegionFullLocalization fullLocalizationOf(WindowRegionHandle h);

        void update(const ls::Rectangle2I& rect);

        WindowRegionHandle queryRegion(const ls::Vec2I& pos);
        ConstWindowRegionHandle queryRegion(const ls::Vec2I& pos) const;

        bool tryDispatchEvent(sf::Event& event, const ls::Vec2I& mousePos);

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    private:
        void setSubdivisionParams(WindowRegionHandle h, const SubdivisionParams& params);
        void update(WindowRegionHandle h, const ls::Rectangle2I& rect);

        template <class EventType>
        void dispatchEvent(EventType& event, SfmlEventHandler::EventResult(SfmlEventHandler::*handler)(EventType&, SfmlEventHandler::EventContext), const ls::Vec2I& mousePos)
        {
            WindowRegionHandle focusedRegionHandle = m_focusedRegionHandle; //stored because can be changed midway
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

            WindowRegionHandle mouseOverRegionHandle = queryRegion(mousePos);
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

            for (WindowRegionHandle h : m_topmostRegions)
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
