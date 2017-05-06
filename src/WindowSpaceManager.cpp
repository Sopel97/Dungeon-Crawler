#include "WindowSpaceManager.h"

#include "WindowSpaceUser.h"

using namespace ls;


WindowSpaceManager::AspectRatio::AspectRatio(float r) :
    m_ratio(r)
{

}
WindowSpaceManager::AspectRatio::AspectRatio(int h, int v) :
    m_ratio(static_cast<float>(h) / v)
{

}

ls::Rectangle2I WindowSpaceManager::AspectRatio::fitRectCentered(const ls::Rectangle2I& space) const
{
    //try full height
    {
        const int fullHeight = space.height();
        const int width = fullHeight * m_ratio;
        const int widthDiff = space.width() - width;
        if (widthDiff >= 0)
            return ls::Rectangle2I::withSize(ls::Vec2I(widthDiff / 2, 0), width, fullHeight);
    }

    //go with full width
    {
        const int fullWidth = space.width();
        const int height = fullWidth / m_ratio;
        const int heightDiff = space.height() - height;

        return ls::Rectangle2I::withSize(ls::Vec2I(0, heightDiff / 2), fullWidth, height);
    }
}

float WindowSpaceManager::AspectRatio::ratio() const
{
    return m_ratio;
}
void WindowSpaceManager::AspectRatio::setRatio(float r)
{
    m_ratio = r;
}
void WindowSpaceManager::AspectRatio::setRatio(int h, int v)
{
    m_ratio = static_cast<float>(h) / v;
}

WindowSpaceManager::SubdivisionParams WindowSpaceManager::SubdivisionParams::
withPixels(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, int pixels)
{
    return SubdivisionParams(orientation, subject, pixels);
}

WindowSpaceManager::SubdivisionParams WindowSpaceManager::SubdivisionParams::
withRatio(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, float ratio)
{
    return SubdivisionParams(orientation, subject, ratio);
}
WindowSpaceManager::SubdivisionParams& WindowSpaceManager::SubdivisionParams::
withAspectRatio(WindowSpaceManager::AspectRatio ratio, WindowSpaceManager::SubdivisionParams::Subject subject)
{
    if (subject == Subject::First) m_aspectRatioFirst = ratio;
    else m_aspectRatioSecond = ratio;

    return *this;
}

std::pair<ls::Rectangle2I, ls::Rectangle2I> WindowSpaceManager::SubdivisionParams::
calculateSubRects(const ls::Rectangle2I& rect) const
{
    const int fullWidth = rect.width();
    const int fullHeight = rect.height();

    int width1 = fullWidth;
    int height1 = fullHeight;
    int width2 = fullWidth;
    int height2 = fullHeight;
    ls::Vec2I offset(0, 0);

    if (m_orientation == Orientation::Horizontal)
    {
        width1 = asPixels(rect);
        width2 = fullWidth - width1;

        if (m_subject == Subject::Second) std::swap(width1, width2);

        offset.x = width1;
    }
    else
    {
        height1 = asPixels(rect);
        height2 = fullHeight - height1;

        if (m_subject == Subject::Second) std::swap(height1, height2);

        offset.y = height1;
    }

    ls::Rectangle2I rect1 = ls::Rectangle2I::withSize(rect.min, width1, height1);
    ls::Rectangle2I rect2 = ls::Rectangle2I::withSize(rect.min + offset, width2, height2);

    if (m_aspectRatioFirst.has_value())
    {
        rect1 = m_aspectRatioFirst.value().fitRectCentered(rect1);
    }
    if (m_aspectRatioSecond.has_value())
    {
        rect2 = m_aspectRatioSecond.value().fitRectCentered(rect2);
    }

    return { rect1, rect2 };
}

int WindowSpaceManager::SubdivisionParams::asPixels(const ls::Rectangle2I& rect) const
{
    if (m_amountType == AmountType::Pixels) return m_amount.pixels;
    else return m_amount.ratio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}
float WindowSpaceManager::SubdivisionParams::asRatio(const ls::Rectangle2I& rect) const
{
    if (m_amountType == AmountType::Ratio) return m_amount.ratio;
    else return static_cast<float>(m_amount.pixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}


void WindowSpaceManager::SubdivisionParams::setPixels(int newPixels)
{
    m_amount.pixels = newPixels;
    m_amountType = AmountType::Pixels;
}

void WindowSpaceManager::SubdivisionParams::setRatio(float newRatio)
{
    m_amount.ratio = newRatio;
    m_amountType = AmountType::Ratio;
}

void WindowSpaceManager::SubdivisionParams::moveByPixels(const ls::Rectangle2I& rect, int deltaPixels)
{
    if (m_amountType == AmountType::Pixels) m_amount.pixels += deltaPixels;
    else
    {
        float deltaRatio = static_cast<float>(deltaPixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
        if (m_subject == Subject::Second) deltaRatio = -deltaRatio;

        m_amount.ratio += deltaRatio;
    }
}
void WindowSpaceManager::SubdivisionParams::moveByRatio(const ls::Rectangle2I& rect, float deltaRatio)
{
    if (m_amountType == AmountType::Ratio) m_amount.ratio += deltaRatio;
    else
    {
        float deltaPixels = deltaRatio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
        if (m_subject == Subject::Second) deltaPixels = -deltaPixels;

        m_amount.pixels = deltaPixels;
    }
}

WindowSpaceManager::SubdivisionParams::
SubdivisionParams(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, int pixels) :
    m_orientation(orientation),
    m_amountType(AmountType::Pixels),
    m_subject(subject),
    m_aspectRatioFirst(std::nullopt),
    m_aspectRatioSecond(std::nullopt)
{
    m_amount.pixels = pixels;
}
WindowSpaceManager::SubdivisionParams::
SubdivisionParams(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, float ratio) :
    m_orientation(orientation),
    m_amountType(AmountType::Ratio),
    m_subject(subject),
    m_aspectRatioFirst(std::nullopt),
    m_aspectRatioSecond(std::nullopt)
{
    m_amount.ratio = ratio;
}

WindowSpaceManager::WindowRegion::
WindowRegion(const ls::Rectangle2I& rect, const std::string& name, Scene& parentScene) :
    m_rect(rect),
    m_name(name),
    m_parentScene(&parentScene),
    m_spaceUser(nullptr)
{

}

const ls::Rectangle2I& WindowSpaceManager::WindowRegion::rect() const
{
    return m_rect;
}
const std::string& WindowSpaceManager::WindowRegion::name() const
{
    return m_name;
}
const WindowSpaceManager::Scene& WindowSpaceManager::WindowRegion::parentScene() const
{
    return *m_parentScene;
}
WindowSpaceManager::Scene& WindowSpaceManager::WindowRegion::parentScene()
{
    return *m_parentScene;
}
bool WindowSpaceManager::WindowRegion::hasEventHandler() const
{
    return m_spaceUser != nullptr;
}
SfmlEventHandler& WindowSpaceManager::WindowRegion::eventHandler()
{
    return *m_spaceUser;
}

void WindowSpaceManager::WindowRegion::setRect(const ls::Rectangle2I& newRect)
{
    m_rect = newRect;
}
void WindowSpaceManager::WindowRegion::setUser(WindowSpaceUser& newUser)
{
    m_spaceUser = &newUser;
}
ls::Vec2I WindowSpaceManager::WindowRegion::localCoords(const ls::Vec2I& windowCoords) const
{
    return windowCoords - m_rect.min;
}
void WindowSpaceManager::WindowRegion::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    if (m_spaceUser != nullptr) m_spaceUser->draw(renderTarget, renderStates);
}

WindowSpaceManager::FreeWindowParams::FreeWindowParams(const ls::Vec2I& defaultSize) :
    m_defaultSize(defaultSize),
    m_minSize(0, 0),
    m_maxSize(std::nullopt),
    m_regionRestriction(std::nullopt)
{

}


WindowSpaceManager::FreeWindowParams& WindowSpaceManager::FreeWindowParams::withMinSize(const ls::Vec2I& minSize)
{
    m_minSize = minSize;
    return *this;
}
WindowSpaceManager::FreeWindowParams& WindowSpaceManager::FreeWindowParams::withMaxSize(const ls::Vec2I& maxSize)
{
    m_maxSize = maxSize;
    return *this;
}
WindowSpaceManager::FreeWindowParams& WindowSpaceManager::FreeWindowParams::withRegionRestriction(ConstBackgroundWindowHandle h)
{
    m_regionRestriction = h;
    return *this;
}

const ls::Vec2I& WindowSpaceManager::FreeWindowParams::defaultSize() const
{
    return m_defaultSize;
}
const ls::Vec2I& WindowSpaceManager::FreeWindowParams::minSize() const
{
    return m_minSize;
}
bool WindowSpaceManager::FreeWindowParams::hasMaxSize() const 
{
    return m_maxSize.has_value();
}
const ls::Vec2I& WindowSpaceManager::FreeWindowParams::maxSize() const
{
    return m_maxSize.value();
}
bool WindowSpaceManager::FreeWindowParams::hasRegionRestriction() const
{
    return m_regionRestriction.has_value();
}
const WindowSpaceManager::ConstBackgroundWindowHandle& WindowSpaceManager::FreeWindowParams::regionRestriction() const
{
    return m_regionRestriction.value();
}

WindowSpaceManager::Scene::Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name) :
    m_windowSpaceManager(&windowSpaceManager),
    m_backgroundWindows(BackgroundWindow(WindowRegion(rect, "Root", *this), std::nullopt)),
    m_name(name),
    m_topmostRegions({ m_backgroundWindows.root() }),
    m_focusedRegionHandle(m_backgroundWindows.root())
{

}

WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(BackgroundWindowHandle h)
{
    return h.data().windowRegion();
}

const WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(ConstBackgroundWindowHandle h) const
{
    return h.data().windowRegion();
}

bool WindowSpaceManager::Scene::isSubdivided(ConstBackgroundWindowHandle h) const
{
    return h.data().params().has_value();
}

WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(BackgroundWindowHandle h)
{
    return h.data().params().value();
}
const WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(ConstBackgroundWindowHandle h) const
{
    return h.data().params().value();
}

std::pair<WindowSpaceManager::BackgroundWindowHandle, WindowSpaceManager::BackgroundWindowHandle> WindowSpaceManager::Scene::
subdivide(BackgroundWindowHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond)
{
    const ls::Rectangle2I& rect = windowRegion(h).rect();
    setSubdivisionParams(h, params);
    auto subdividedRects = params.calculateSubRects(rect);

    BackgroundWindowHandle left = m_backgroundWindows.emplaceLeft(h, WindowRegion(subdividedRects.first, nameFirst, *this), std::nullopt);
    BackgroundWindowHandle right = m_backgroundWindows.emplaceRight(h, WindowRegion(subdividedRects.second, nameSecond, *this), std::nullopt);

    auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
    *it = left;	// it is guaranteed to exist
    m_topmostRegions.push_back(right);

    return { left, right };
}
WindowSpaceManager::FreeWindowHandle WindowSpaceManager::Scene::createFreeWindow(const FreeWindowParams& params, const std::string& name, const ls::Vec2I& center)
{
    const ls::Vec2I& size = params.defaultSize();
    const ls::Vec2I localCenter = [&]() {
        if (params.hasRegionRestriction())
        {
            const auto& region = params.regionRestriction().data().windowRegion();
            return center + region.rect().min;
        }
        else return center;
    }();

    const ls::Vec2I topLeft = localCenter - size / 2;
    const ls::Rectangle2I rect(topLeft, topLeft + size);

    m_freeWindows.emplace_back(WindowRegion(rect, name, *this), params);
    return std::prev(m_freeWindows.end());
}

void WindowSpaceManager::Scene::
setSubdivisionParams(WindowSpaceManager::BackgroundWindowHandle h, const WindowSpaceManager::SubdivisionParams& params)
{
    h.data().setParams(params);
}

WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::firstChild(BackgroundWindowHandle h)
{
    return h.left();
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::firstChild(ConstBackgroundWindowHandle h) const
{
    return h.left();
}
WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::secondChild(BackgroundWindowHandle h)
{
    return h.right();
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::secondChild(ConstBackgroundWindowHandle h) const
{
    return h.right();
}
WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::parent(BackgroundWindowHandle h)
{
    return h.parent();
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::parent(ConstBackgroundWindowHandle h) const
{
    return h.parent();
}
const std::string& WindowSpaceManager::Scene::name() const
{
    return m_name;
}
bool WindowSpaceManager::Scene::hasChildren(ConstBackgroundWindowHandle h) const
{
    return h.hasLeft();	// should always have right too
}
bool WindowSpaceManager::Scene::hasParent(ConstBackgroundWindowHandle h) const
{
    return h.hasParent();
}
WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::rootHandle()
{
    return m_backgroundWindows.root();
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::rootHandle() const
{
    return m_backgroundWindows.root();
}

WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::findBackgroundWindow(const std::string& name)
{
    return m_backgroundWindows.findIf([&name](const BackgroundWindow& window)->bool {return window.windowRegion().name() == name; });
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::findBackgroundWindow(const std::string& name) const
{
    return m_backgroundWindows.findIf([&name](const BackgroundWindow& window)->bool {return window.windowRegion().name() == name; });
}
WindowSpaceManager::WindowRegionFullLocalization WindowSpaceManager::Scene::fullLocalizationOf(BackgroundWindowHandle h)
{
    return { m_windowSpaceManager, this, &(h.data().windowRegion()) };
}

void WindowSpaceManager::Scene::update(const ls::Rectangle2I& rect)
{
    update(m_backgroundWindows.root(), rect);
}
void WindowSpaceManager::Scene::update(BackgroundWindowHandle h, const ls::Rectangle2I& rect)
{
    windowRegion(h).setRect(rect);
    if (!isSubdivided(h)) return;

    auto subdividedRects = subdivisionParams(h).calculateSubRects(rect);
    if (hasChildren(h))
    {
        update(h.left(), subdividedRects.first);
        update(h.right(), subdividedRects.second);
    }
}

WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::queryRegion(const ls::Vec2I& pos)
{
    auto iter = const_cast<const Scene*>(this)->queryRegion(pos);

    //dirty hack for now TODO: this better
    return BackgroundWindowHandle(
        const_cast<BackgroundWindowsStorage&>(iter.tree()),
        const_cast<BackgroundWindowsStorage::Node*>(iter.node())
    );
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::queryRegion(const ls::Vec2I& pos) const
{
    ConstBackgroundWindowHandle currentRegionHandle = m_backgroundWindows.root();

    // only leaves can be returned
    for (;;)
    {
        // we know it has both left and right child

        if (!ls::intersect(windowRegion(currentRegionHandle).rect(), pos))
        {
            return {};
        }

        if (!hasChildren(currentRegionHandle)) break;

        ConstBackgroundWindowHandle leftChildHandle = currentRegionHandle.left();
        ConstBackgroundWindowHandle rightChildHandle = currentRegionHandle.right();

        //explore further the intersecting region
        if (ls::intersect(windowRegion(leftChildHandle).rect(), pos))
        {
            currentRegionHandle = leftChildHandle;
        }
        else if (ls::intersect(windowRegion(rightChildHandle).rect(), pos))
        {
            currentRegionHandle = rightChildHandle;
        }
        else return {}; //should not happen
    }

    return currentRegionHandle;
}

bool WindowSpaceManager::Scene::tryDispatchEvent(sf::Event& event, const Vec2I& mousePos)
{
    switch (event.type)
    {
    case sf::Event::TextEntered:
        dispatchEvent(event.text, &SfmlEventHandler::onTextEntered, mousePos);
        return true;

    case sf::Event::KeyPressed:
        dispatchEvent(event.key, &SfmlEventHandler::onKeyPressed, mousePos);
        return true;

    case sf::Event::KeyReleased:
        dispatchEvent(event.key, &SfmlEventHandler::onKeyReleased, mousePos);
        return true;

    case sf::Event::MouseWheelMoved:
        dispatchEvent(event.mouseWheel, &SfmlEventHandler::onMouseWheelMoved, mousePos);
        return true;

    case sf::Event::MouseButtonPressed:
        dispatchEvent(event.mouseButton, &SfmlEventHandler::onMouseButtonPressed, mousePos);
        return true;

    case sf::Event::MouseButtonReleased:
        dispatchEvent(event.mouseButton, &SfmlEventHandler::onMouseButtonReleased, mousePos);
        return true;

    case sf::Event::MouseMoved:
        dispatchEvent(event.mouseMove, &SfmlEventHandler::onMouseMoved, mousePos);
        return true;

    default:
        return false;
    }
}
void WindowSpaceManager::Scene::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for (BackgroundWindowHandle h : m_topmostRegions)
    {
        WindowRegion& region = windowRegion(h);
        region.draw(renderTarget, renderStates);
    }
}

WindowSpaceManager::WindowSpaceManager(sf::RenderWindow& window) :
    m_window(window),
    m_scenes(),
    m_currentScene(nullptr)
{

}
auto WindowSpaceManager::createScene(const std::string& name)
-> Scene&
{
    return m_scenes.try_emplace(name, *this, rect(), name).first->second;
}

WindowSpaceManager::Scene& WindowSpaceManager::scene(const std::string& name)
{
    return m_scenes.at(name);
}
const WindowSpaceManager::Scene& WindowSpaceManager::scene(const std::string& name) const
{
    return m_scenes.at(name);
}

void WindowSpaceManager::changeScene(const std::string& name)
{
    m_currentScene = &(scene(name));
    m_currentScene->update(rect());
}
ls::Rectangle2I WindowSpaceManager::rect() const
{
    return ls::Rectangle2I::withSize(ls::Vec2I(0, 0), m_window.getSize().x, m_window.getSize().y);
}

void WindowSpaceManager::onWindowResized()
{
    m_currentScene->update(rect());
}

sf::View WindowSpaceManager::getRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const
{
    sf::View view(sf::FloatRect(worldRect.min.x, worldRect.min.y, worldRect.width(), worldRect.height()));
    view.setViewport(viewportConvertToRatio(viewport));
    return view;
}
sf::View WindowSpaceManager::getRegionView(const WindowSpaceManager::WindowRegion& region, const ls::Rectangle2F& worldRect) const
{
    return getRectView(region.rect(), worldRect);
}
void WindowSpaceManager::setRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const
{
    m_window.setView(getRectView(viewport, worldRect));
}
void WindowSpaceManager::setRegionView(const WindowSpaceManager::WindowRegion& region, const ls::Rectangle2F& worldRect) const
{
    m_window.setView(getRegionView(region, worldRect));
}
void WindowSpaceManager::setView(const sf::View& view)
{
    m_window.setView(view);
}
void WindowSpaceManager::setDefaultView()
{
    setRectView(rect(), static_cast<Rectangle2F>(rect()));
}
bool WindowSpaceManager::tryDispatchEvent(sf::Event& event)
{
    const sf::Vector2i m = sf::Mouse::getPosition(m_window);
    const Vec2I mousePos{ m.x, m.y };

    return m_currentScene->tryDispatchEvent(event, mousePos);
}

void WindowSpaceManager::drawCurrentScene(sf::RenderStates& renderStates)
{
    m_currentScene->draw(m_window, renderStates);
}

sf::RenderWindow& WindowSpaceManager::window()
{
    return m_window;
}
sf::FloatRect WindowSpaceManager::viewportConvertToRatio(const ls::Rectangle2I& rect) const
{
    float windowWidth = m_window.getSize().x;
    float windowHeight = m_window.getSize().y;
    return sf::FloatRect(rect.min.x / windowWidth, rect.min.y / windowHeight, rect.width() / windowWidth, rect.height() / windowHeight);
}