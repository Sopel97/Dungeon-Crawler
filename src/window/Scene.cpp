#include "window/Scene.h"

using namespace ls;

Scene::Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name) :
    m_windowSpaceManager(&windowSpaceManager),
    m_backgroundWindows(windowSpaceManager, rect, "Root"),
    m_name(name),
    m_topmostRegions({ m_backgroundWindows.root() }),
    m_focusedWindow(&(m_backgroundWindows.root().data()))
{

}

BackgroundWindow& Scene::window(BackgroundWindowHandle h)
{
    return h.data();
}

const BackgroundWindow& Scene::window(ConstBackgroundWindowHandle h) const
{
    return h.data();
}

bool Scene::isSubdivided(ConstBackgroundWindowHandle h) const
{
    return h.data().params().has_value();
}

RectSubdivision& Scene::subdivisionParams(BackgroundWindowHandle h)
{
    return h.data().params().value();
}
const RectSubdivision& Scene::subdivisionParams(ConstBackgroundWindowHandle h) const
{
    return h.data().params().value();
}

std::pair<Scene::BackgroundWindowHandle, Scene::BackgroundWindowHandle> Scene::
subdivide(BackgroundWindowHandle h, const RectSubdivision& params, const std::string& nameFirst, const std::string& nameSecond)
{
    const ls::Rectangle2I& rect = window(h).absoluteContentRect();
    setSubdivisionParams(h, params);
    // they have to be translated to local coordinates, so (0,0) is the top left
    auto subdividedRects = params.calculateSubRects(rect.translated(-rect.min));

    BackgroundWindowHandle left = m_backgroundWindows.emplaceLeft(h, *m_windowSpaceManager, subdividedRects.first, nameFirst, window(h));
    BackgroundWindowHandle right = m_backgroundWindows.emplaceRight(h, *m_windowSpaceManager, subdividedRects.second, nameSecond, window(h));

    auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
    *it = left;	// it is guaranteed to exist
    m_topmostRegions.push_back(right);

    return { left, right };
}
Scene::FreeWindowHandle Scene::addFreeWindow(std::unique_ptr<InternalWindow>&& wnd)
{
    m_focusedWindow = wnd.get();
    m_freeWindows.emplace_back(std::move(wnd));
    return std::prev(m_freeWindows.end());
}

void Scene::setSubdivisionParams(BackgroundWindowHandle h, const RectSubdivision& params)
{
    h.data().setParams(params);
}

Scene::BackgroundWindowHandle Scene::firstChild(BackgroundWindowHandle h)
{
    return h.left();
}
Scene::ConstBackgroundWindowHandle Scene::firstChild(ConstBackgroundWindowHandle h) const
{
    return h.left();
}
Scene::BackgroundWindowHandle Scene::secondChild(BackgroundWindowHandle h)
{
    return h.right();
}
Scene::ConstBackgroundWindowHandle Scene::secondChild(ConstBackgroundWindowHandle h) const
{
    return h.right();
}
Scene::BackgroundWindowHandle Scene::parent(BackgroundWindowHandle h)
{
    return h.parent();
}
Scene::ConstBackgroundWindowHandle Scene::parent(ConstBackgroundWindowHandle h) const
{
    return h.parent();
}
const std::string& Scene::name() const
{
    return m_name;
}
bool Scene::hasChildren(ConstBackgroundWindowHandle h) const
{
    return h.hasLeft();	// should always have right too
}
bool Scene::hasParent(ConstBackgroundWindowHandle h) const
{
    return h.hasParent();
}
Scene::BackgroundWindowHandle Scene::rootHandle()
{
    return m_backgroundWindows.root();
}
Scene::ConstBackgroundWindowHandle Scene::rootHandle() const
{
    return m_backgroundWindows.root();
}

Scene::BackgroundWindowHandle Scene::findBackgroundWindow(const std::string& title)
{
    return m_backgroundWindows.findIf([&title](const BackgroundWindow& window)->bool {return window.title() == title; });
}
Scene::ConstBackgroundWindowHandle Scene::findBackgroundWindow(const std::string& title) const
{
    return m_backgroundWindows.findIf([&title](const BackgroundWindow& window)->bool {return window.title() == title; });
}

void Scene::update(const ls::Rectangle2I& rect)
{
    update(m_backgroundWindows.root(), rect);
}
void Scene::update(BackgroundWindowHandle h, const ls::Rectangle2I& rect)
{
    window(h).setWindowRect(rect);
    if (!isSubdivided(h)) return;

    // they have to be translated to local coordinates, so (0,0) is the top left
    auto contentRect = window(h).contentRect();
    auto subdividedRects = subdivisionParams(h).calculateSubRects(contentRect.translated(-contentRect.min));
    if (hasChildren(h))
    {
        update(h.left(), subdividedRects.first);
        update(h.right(), subdividedRects.second);
    }
}
void Scene::dispatchEvent(sf::Event& event, const ls::Vec2I& mousePos)
{
    FreeWindowHandle mouseOverFreeWindowHandle = queryFreeWindow(mousePos);
    if (mouseOverFreeWindowHandle != m_freeWindows.end())
    {
        InternalWindow& mouseOverWindow = *(*mouseOverFreeWindowHandle);
        bool isFocused = &mouseOverWindow == m_focusedWindow;

        const SfmlEventHandler::EventContext context = SfmlEventHandler::EventContext{}.setIsFocused(isFocused).setIsMouseOver();
        const SfmlEventHandler::EventResult result = mouseOverWindow.dispatch(event, context, mousePos);
        if (result.takeFocus)
        {
            m_focusedWindow = &mouseOverWindow;
        }
        if (result.consumeEvent)
        {
            return;
        }
    }

    BackgroundWindowHandle mouseOverRegionHandle = queryBackgroundWindow(mousePos);
    if (mouseOverFreeWindowHandle == m_freeWindows.end() && mouseOverRegionHandle.isValid())
    {
        InternalWindow& mouseOverRegion = window(mouseOverRegionHandle);
        bool isFocused = &mouseOverRegion == m_focusedWindow;

        const SfmlEventHandler::EventContext context = SfmlEventHandler::EventContext{}.setIsFocused(isFocused).setIsMouseOver();
        const SfmlEventHandler::EventResult result = mouseOverRegion.dispatch(event, context, mousePos);
        if (result.takeFocus)
        {
            m_focusedWindow = &mouseOverRegion;
        }
        if (result.consumeEvent)
        {
            return;
        }
    }

    InternalWindow* focusedWindow = m_focusedWindow; //stored because can be changed midway
    if (m_focusedWindow != nullptr && &(mouseOverRegionHandle.data()) != m_focusedWindow)
    {
        InternalWindow& focused = *m_focusedWindow;
        const SfmlEventHandler::EventContext context = SfmlEventHandler::EventContext{}
            .setIsFocused()
            .setIsMouseOver(false);
        const SfmlEventHandler::EventResult result = focused.dispatch(event, context, mousePos);
        if (result.consumeEvent)
        {
            return;
        }
    }

    // other regions
    for (auto iter = m_freeWindows.rbegin(); iter != m_freeWindows.rend(); ++iter)
    {
        InternalWindow& wnd = **iter;
        if (&wnd == focusedWindow || &wnd == mouseOverFreeWindowHandle->get()) continue;

        const SfmlEventHandler::EventContext context = SfmlEventHandler::EventContext{}.setIsFocused(false).setIsMouseOver(false);
        const SfmlEventHandler::EventResult result = wnd.dispatch(event, context, mousePos);

        if (result.takeFocus)
        {
            m_focusedWindow = &wnd;
        }
        if (result.consumeEvent)
        {
            return;
        }
    }

    for (BackgroundWindowHandle h : m_topmostRegions)
    {
        if (&(h.data()) == focusedWindow || h == mouseOverRegionHandle) continue;

        BackgroundWindow& region = window(h);
        const SfmlEventHandler::EventContext context = SfmlEventHandler::EventContext{}.setIsFocused(false).setIsMouseOver(false);
        const SfmlEventHandler::EventResult result = region.dispatch(event, context, mousePos);
        if (result.takeFocus)
        {
            m_focusedWindow = &(h.data());
        }
        if (result.consumeEvent)
        {
            return;
        }
    }

    moveFocusedFreeWindowToTop();
}

Scene::BackgroundWindowHandle Scene::queryBackgroundWindow(const ls::Vec2I& pos)
{
    auto iter = const_cast<const Scene*>(this)->queryBackgroundWindow(pos);
    if (!iter.isValid()) return {};

    //dirty hack for now TODO: this better
    return findBackgroundWindow(iter.data().title());
}
Scene::ConstBackgroundWindowHandle Scene::queryBackgroundWindow(const ls::Vec2I& pos) const
{
    ConstBackgroundWindowHandle currentRegionHandle = m_backgroundWindows.root();

    // only leaves can be returned
    for (;;)
    {
        // we know it has both left and right child

        if (!ls::intersect(window(currentRegionHandle).absoluteWindowRect(), pos))
        {
            return {};
        }

        if (!hasChildren(currentRegionHandle)) break;

        ConstBackgroundWindowHandle leftChildHandle = currentRegionHandle.left();
        ConstBackgroundWindowHandle rightChildHandle = currentRegionHandle.right();

        //explore further the intersecting region
        if (ls::intersect(window(leftChildHandle).absoluteWindowRect(), pos))
        {
            currentRegionHandle = leftChildHandle;
        }
        else if (ls::intersect(window(rightChildHandle).absoluteWindowRect(), pos))
        {
            currentRegionHandle = rightChildHandle;
        }
        else return {}; //should not happen
    }

    return currentRegionHandle;
}
Scene::FreeWindowHandle Scene::queryFreeWindow(const ls::Vec2I& pos)
{
    for (auto iter = m_freeWindows.begin(); iter != m_freeWindows.end(); ++iter)
    {
        auto& wnd = **iter;
        if (ls::intersect(wnd.absoluteWindowRect(), pos)) return iter;
    }

    return m_freeWindows.end();
}

bool Scene::tryDispatchEvent(sf::Event& event, const Vec2I& mousePos)
{
    switch (event.type)
    {
    case sf::Event::TextEntered:
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
    case sf::Event::MouseWheelMoved:
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
    case sf::Event::MouseMoved:
        dispatchEvent(event, mousePos);
        return true;

    default:
        return false;
    }
}
void Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for (BackgroundWindowHandle h : m_topmostRegions)
    {
        BackgroundWindow& wnd = window(h);
        wnd.draw(renderTarget, renderStates);
    }

    for (auto& wnd : m_freeWindows)
    {
        wnd->draw(renderTarget, renderStates);
    }
}
void Scene::removeClosingFreeWindows()
{
    m_freeWindows.erase(std::remove_if(m_freeWindows.begin(), m_freeWindows.end(), [](const std::unique_ptr<InternalWindow>& wnd)->bool {return wnd->isClosing(); }), m_freeWindows.end());
    if (std::none_of(m_freeWindows.begin(), m_freeWindows.end(), [this](const std::unique_ptr<InternalWindow>& wnd)->bool {return wnd.get() == this->m_focusedWindow; }))
    {
        m_focusedWindow = nullptr;
    }
}
void Scene::moveFocusedFreeWindowToTop()
{
    for (auto iter = m_freeWindows.begin(); iter != m_freeWindows.end(); ++iter)
    {
        auto& wnd = **iter;
        if (&wnd == m_focusedWindow)
        {
            m_freeWindows.splice(m_freeWindows.end(), m_freeWindows, iter);
            return;
        }
    }
}