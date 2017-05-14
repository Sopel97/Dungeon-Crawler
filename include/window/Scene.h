#pragma once

#include "../LibS/Geometry.h"

#include "BackgroundWindow.h"
#include "FreeWindow.h"

class WindowSpaceManager;

class Scene
{
public:
    using BackgroundWindowsStorage = ls::BinaryTree<BackgroundWindow>;
    using BackgroundWindowHandle = typename BackgroundWindowsStorage::Iterator;
    using ConstBackgroundWindowHandle = typename BackgroundWindowsStorage::ConstIterator;

    using FreeWindowStorage = std::list<FreeWindow>;
    using FreeWindowHandle = typename FreeWindowStorage::iterator;
    using ConstFreeWindowHandle = typename FreeWindowStorage::const_iterator;
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

    RectSubdivision& subdivisionParams(BackgroundWindowHandle h);
    const RectSubdivision& subdivisionParams(ConstBackgroundWindowHandle h) const;

    std::pair<BackgroundWindowHandle, BackgroundWindowHandle> subdivide(BackgroundWindowHandle h, const RectSubdivision& params, const std::string& nameFirst, const std::string& nameSecond);
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

    BackgroundWindowHandle queryBackgroundWindow(const ls::Vec2I& pos);
    ConstBackgroundWindowHandle queryBackgroundWindow(const ls::Vec2I& pos) const;

    bool tryDispatchEvent(sf::Event& event, const ls::Vec2I& mousePos);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

private:
    void setSubdivisionParams(BackgroundWindowHandle h, const RectSubdivision& params);
    void update(BackgroundWindowHandle h, const ls::Rectangle2I& rect);

    template <class EventType>
    void dispatchEvent(EventType& event, SfmlEventHandler::EventResult(SfmlEventHandler::*handler)(EventType&, SfmlEventHandler::EventContext), const ls::Vec2I& mousePos)
    {
        BackgroundWindowHandle focusedRegionHandle = m_focusedRegionHandle; //stored because can be changed midway
        InternalWindow& focused = window(focusedRegionHandle);
        const SfmlEventHandler::EventContext context{ true, ls::intersect(focused.absoluteWindowRect(), mousePos) };
        const SfmlEventHandler::EventResult result = (focused.*handler)(event, context);
        if (result.consumeEvent)
        {
            return;
        }

        // other regions

        BackgroundWindowHandle mouseOverRegionHandle = queryBackgroundWindow(mousePos);
        if (mouseOverRegionHandle.isValid() && mouseOverRegionHandle != m_focusedRegionHandle)
        {
            InternalWindow& mouseOverRegion = window(mouseOverRegionHandle);

            const SfmlEventHandler::EventContext context{ false, true };
            const SfmlEventHandler::EventResult result = (mouseOverRegion.*handler)(event, context);
            if (result.takeFocus)
            {
                m_focusedRegionHandle = mouseOverRegionHandle;
            }
            if (result.consumeEvent)
            {
                return;
            }
        }

        for (BackgroundWindowHandle h : m_topmostRegions)
        {
            if (h == focusedRegionHandle || h == mouseOverRegionHandle) continue;

            BackgroundWindow& region = window(h);
            const SfmlEventHandler::EventContext context{ false, false };
            const SfmlEventHandler::EventResult result = (region.*handler)(event, context);
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
};