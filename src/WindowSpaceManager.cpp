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


WindowSpaceManager::Scene::Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name) :
    m_windowSpaceManager(&windowSpaceManager),
    m_windowRegions(std::make_pair(WindowRegion(rect, "Root", *this), std::nullopt)),
    m_name(name),
    m_topmostRegions({ m_windowRegions.root() }),
    m_focusedRegionHandle(m_windowRegions.root())
{

}
WindowSpaceManager::Scene::Scene(WindowSpaceManager::Scene&& other) :
    m_windowSpaceManager(other.m_windowSpaceManager),
    m_windowRegions(std::move(other.m_windowRegions)),
    m_name(std::move(other.m_name)),
    m_topmostRegions(std::move(other.m_topmostRegions)),
    m_focusedRegionHandle(other.m_focusedRegionHandle)
{

}
WindowSpaceManager::Scene& WindowSpaceManager::Scene::operator=(WindowSpaceManager::Scene&& other)
{
    m_windowSpaceManager = other.m_windowSpaceManager;
    m_windowRegions = std::move(other.m_windowRegions);
    m_name = std::move(other.m_name);
    m_topmostRegions = std::move(other.m_topmostRegions);
    m_focusedRegionHandle = other.m_focusedRegionHandle;

    return *this;
}

WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(WindowRegionHandle h)
{
    return h.data().first;
}

const WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(ConstWindowRegionHandle h) const
{
    return h.data().first;
}

bool WindowSpaceManager::Scene::isSubdivided(ConstWindowRegionHandle h) const
{
    return h.data().second.has_value();
}

WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(WindowRegionHandle h)
{
    return h.data().second.value();
}
const WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(ConstWindowRegionHandle h) const
{
    return h.data().second.value();
}

std::pair<WindowSpaceManager::WindowRegionHandle, WindowSpaceManager::WindowRegionHandle> WindowSpaceManager::Scene::
subdivide(WindowRegionHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond)
{
    const ls::Rectangle2I& rect = windowRegion(h).rect();
    setSubdivisionParams(h, params);
    auto subdividedRects = params.calculateSubRects(rect);

    WindowRegionHandle left = m_windowRegions.emplaceLeft(h,
        std::piecewise_construct,
        std::forward_as_tuple(subdividedRects.first, nameFirst, *this),
        std::forward_as_tuple(std::nullopt)
    );
    WindowRegionHandle right = m_windowRegions.emplaceRight(h,
        std::piecewise_construct,
        std::forward_as_tuple(subdividedRects.second, nameSecond, *this),
        std::forward_as_tuple(std::nullopt)
    );

    auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
    *it = left;	// it is guaranteed to exist
    m_topmostRegions.push_back(right);

    return { left, right };
}

void WindowSpaceManager::Scene::
setSubdivisionParams(WindowSpaceManager::WindowRegionHandle h, const WindowSpaceManager::SubdivisionParams& params)
{
    h.data().second.emplace(params);
}

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::firstChild(WindowRegionHandle h)
{
    return h.left();
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::firstChild(ConstWindowRegionHandle h) const
{
    return h.left();
}
WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::secondChild(WindowRegionHandle h)
{
    return h.right();
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::secondChild(ConstWindowRegionHandle h) const
{
    return h.right();
}
WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::parent(WindowRegionHandle h)
{
    return h.parent();
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::parent(ConstWindowRegionHandle h) const
{
    return h.parent();
}
const std::string& WindowSpaceManager::Scene::name() const
{
    return m_name;
}
bool WindowSpaceManager::Scene::hasChildren(ConstWindowRegionHandle h) const
{
    return h.hasLeft();	// should always have right too
}
bool WindowSpaceManager::Scene::hasParent(ConstWindowRegionHandle h) const
{
    return h.hasParent();
}
WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::rootHandle()
{
    return m_windowRegions.root();
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::rootHandle() const
{
    return m_windowRegions.root();
}

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::find(const std::string& name)
{
    return m_windowRegions.findIf([&name](const WindowRegionStorageNode& node)->bool {return node.first.name() == name; });
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::find(const std::string& name) const
{
    return m_windowRegions.findIf([&name](const WindowRegionStorageNode& node)->bool {return node.first.name() == name; });
}
WindowSpaceManager::WindowRegionFullLocalization WindowSpaceManager::Scene::fullLocalizationOf(WindowRegionHandle h)
{
    return { m_windowSpaceManager, this, h };
}

void WindowSpaceManager::Scene::update(const ls::Rectangle2I& rect)
{
    update(m_windowRegions.root(), rect);
}
void WindowSpaceManager::Scene::update(WindowRegionHandle h, const ls::Rectangle2I& rect)
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

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::queryRegion(const ls::Vec2I& pos)
{
    auto iter = const_cast<const Scene*>(this)->queryRegion(pos);

    //dirty hack for now TODO: this better
    return WindowRegionHandle(
        const_cast<WindowRegionStorage&>(iter.tree()),
        const_cast<WindowRegionStorage::Node*>(iter.node())
    );
}
WindowSpaceManager::ConstWindowRegionHandle WindowSpaceManager::Scene::queryRegion(const ls::Vec2I& pos) const
{
    ConstWindowRegionHandle currentRegionHandle = m_windowRegions.root();

    // only leaves can be returned
    for (;;)
    {
        // we know it has both left and right child

        if (!ls::intersect(windowRegion(currentRegionHandle).rect(), pos))
        {
            return {};
        }

        if (!hasChildren(currentRegionHandle)) break;

        ConstWindowRegionHandle leftChildHandle = currentRegionHandle.left();
        ConstWindowRegionHandle rightChildHandle = currentRegionHandle.right();

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
    for (WindowRegionHandle h : m_topmostRegions)
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