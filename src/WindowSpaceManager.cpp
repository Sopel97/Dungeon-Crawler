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

WindowSpaceManager::Window::Window(const ls::Rectangle2I& windowRect, const std::string& name) :
    m_windowRect(windowRect),
    m_name(name),
    m_spaceUser(nullptr)
{

}

WindowSpaceManager::Window::~Window()
{

}

const int WindowSpaceManager::Window::m_playerUiPanelWidth = 230;
const int WindowSpaceManager::Window::m_windowTopBarHeight = 4;
const int WindowSpaceManager::Window::m_windowHeaderHeight = 15;
const int WindowSpaceManager::Window::m_windowLeftBarWidth = 4;
const int WindowSpaceManager::Window::m_windowBottomBarHeight = 4;
const int WindowSpaceManager::Window::m_windowRightBarWidth = 4;
const int WindowSpaceManager::Window::m_windowScrollBarWidth = 12;
const Vec2I WindowSpaceManager::Window::m_windowHighTopLeftCornerSize = { m_windowLeftBarWidth, m_windowHeaderHeight };
const Vec2I WindowSpaceManager::Window::m_windowLowTopLeftCornerSize = { m_windowLeftBarWidth, m_windowTopBarHeight };
const Vec2I WindowSpaceManager::Window::m_windowHighTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I WindowSpaceManager::Window::m_windowLowTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I WindowSpaceManager::Window::m_windowBottomLeftCornerSize = { m_windowLeftBarWidth, m_windowBottomBarHeight };
const Vec2I WindowSpaceManager::Window::m_windowBottomRightCornerSize = { m_windowRightBarWidth, m_windowBottomBarHeight };
const Vec2I WindowSpaceManager::Window::m_windowButtonSize = { 12, 12 };
const Vec2I WindowSpaceManager::Window::m_windowScrollSliderSize = { 12, 12 };

const Vec2I WindowSpaceManager::Window::m_windowTopBarSpritePosition = { 0, 0 };
const Vec2I WindowSpaceManager::Window::m_windowHeaderSpritePosition = { 0, 8 };
const Vec2I WindowSpaceManager::Window::m_windowLeftBarSpritePosition = { 0, 0 };
const Vec2I WindowSpaceManager::Window::m_windowRightBarSpritePosition = { 4, 0 };
const Vec2I WindowSpaceManager::Window::m_windowBottomBarSpritePosition = { 0, 4 };
const Vec2I WindowSpaceManager::Window::m_windowHighTopLeftCornerSpritePosition = { 206, 19 };
const Vec2I WindowSpaceManager::Window::m_windowLowTopLeftCornerSpritePosition = { 198, 19 };
const Vec2I WindowSpaceManager::Window::m_windowHighTopRightCornerSpritePosition = { 210, 19 };
const Vec2I WindowSpaceManager::Window::m_windowLowTopRightCornerSpritePosition = { 202, 19 };
const Vec2I WindowSpaceManager::Window::m_windowBottomLeftCornerSpritePosition = { 198, 23 };
const Vec2I WindowSpaceManager::Window::m_windowBottomRightCornerSpritePosition = { 202, 23 };
const Vec2I WindowSpaceManager::Window::m_windowScrollBarSpritePosition = { 8, 0 };
const Vec2I WindowSpaceManager::Window::m_windowScrollBarUpButtonSpritePosition = { 0, 57 };
const Vec2I WindowSpaceManager::Window::m_windowScrollBarDownButtonSpritePosition = { 12, 57 };
const Vec2I WindowSpaceManager::Window::m_windowScrollBarSliderSpritePosition = { 24, 57 };

const ls::Rectangle2I& WindowSpaceManager::Window::windowRect() const
{
    return m_windowRect;
}
ls::Rectangle2I WindowSpaceManager::Window::contentRect() const
{
    ls::Rectangle2I rect = m_windowRect;
    ls::Vec2I offsetFromTopLeft(m_windowLeftBarWidth, m_windowTopBarHeight);
    if (this->hasHeader())
    {
        offsetFromTopLeft.y = m_windowHeaderHeight;
    }

    ls::Vec2I offsetFromBottomRight(-m_windowRightBarWidth, -m_windowBottomBarHeight);
    if (this->hasScrollBar())
    {
        offsetFromBottomRight.x = -m_windowScrollBarWidth;
    }

    return ls::Rectangle2I(rect.min + offsetFromTopLeft, rect.max + offsetFromBottomRight);
}
const std::string& WindowSpaceManager::Window::title() const
{
    return m_name;
}

void WindowSpaceManager::Window::setWindowRect(const ls::Rectangle2I& newRect)
{
    m_windowRect = newRect;
}
void WindowSpaceManager::Window::setWindowPosition(const ls::Vec2I& newPosition)
{
    m_windowRect.translate(newPosition - m_windowRect.min);
}
void WindowSpaceManager::Window::setWindowSize(const ls::Vec2I& newSize)
{
    m_windowRect.max = m_windowRect.min + newSize;
}

ls::Vec2I WindowSpaceManager::Window::minContentSize() const
{
    return { 0, 0 };
}
bool WindowSpaceManager::Window::hasMaxContentSize() const
{
    return false;
}
ls::Vec2I WindowSpaceManager::Window::maxContentSize() const
{
    return { 0, 0 };
}

int WindowSpaceManager::Window::verticalScroll() const
{
    return 0;
}

bool WindowSpaceManager::Window::isMinimizable() const
{
    return false;
}
bool WindowSpaceManager::Window::isCloseable() const
{
    return false;
}
bool WindowSpaceManager::Window::isResizeable() const
{
    return false;
}
bool WindowSpaceManager::Window::isMovable() const
{
    return false;
}

bool WindowSpaceManager::Window::hasHeader() const
{
    return false;
}
bool WindowSpaceManager::Window::hasScrollBar() const
{
    return false;
}

bool WindowSpaceManager::Window::hasEventHandler() const
{
    return m_spaceUser != nullptr;
}
SfmlEventHandler& WindowSpaceManager::Window::eventHandler()
{
    return *m_spaceUser;
}

void WindowSpaceManager::Window::setUser(WindowSpaceUser& newUser)
{
    m_spaceUser = &newUser;
}

ls::Vec2I WindowSpaceManager::Window::localWindowCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - m_windowRect.min;
}
ls::Vec2I WindowSpaceManager::Window::localContentCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - this->contentRect().min;
}

void WindowSpaceManager::Window::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    if (m_spaceUser != nullptr) m_spaceUser->draw(renderTarget, renderStates);
}

WindowSpaceManager::FreeWindowParams::FreeWindowParams(const ls::Vec2I& defaultSize) :
    m_defaultSize(defaultSize),
    m_minSize(0, 0),
    m_maxSize(std::nullopt),
    m_parentWindow(std::nullopt)
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
WindowSpaceManager::FreeWindowParams& WindowSpaceManager::FreeWindowParams::withParentWindow(const Window& window)
{
    m_parentWindow = &window;
    return *this;
}
WindowSpaceManager::FreeWindowParams& WindowSpaceManager::FreeWindowParams::withHeader()
{
    m_showHeader = true;
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
bool WindowSpaceManager::FreeWindowParams::hasParentWindow() const
{
    return m_parentWindow.has_value();
}
const WindowSpaceManager::Window& WindowSpaceManager::FreeWindowParams::parentWindow() const
{
    return *(m_parentWindow.value());
}
bool WindowSpaceManager::FreeWindowParams::hasHeader() const
{
    return m_showHeader;
}

WindowSpaceManager::Scene::Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name) :
    m_windowSpaceManager(&windowSpaceManager),
    m_backgroundWindows(BackgroundWindow(rect, "Root")),
    m_name(name),
    m_topmostRegions({ m_backgroundWindows.root() }),
    m_focusedRegionHandle(m_backgroundWindows.root())
{

}

WindowSpaceManager::BackgroundWindow& WindowSpaceManager::Scene::window(BackgroundWindowHandle h)
{
    return h.data();
}

const WindowSpaceManager::BackgroundWindow& WindowSpaceManager::Scene::window(ConstBackgroundWindowHandle h) const
{
    return h.data();
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
    const ls::Rectangle2I& rect = window(h).windowRect();
    setSubdivisionParams(h, params);
    auto subdividedRects = params.calculateSubRects(rect);

    BackgroundWindowHandle left = m_backgroundWindows.emplaceLeft(h, subdividedRects.first, nameFirst);
    BackgroundWindowHandle right = m_backgroundWindows.emplaceRight(h, subdividedRects.second, nameSecond);

    auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
    *it = left;	// it is guaranteed to exist
    m_topmostRegions.push_back(right);

    return { left, right };
}
WindowSpaceManager::FreeWindowHandle WindowSpaceManager::Scene::createFreeWindow(const FreeWindowParams& params, const std::string& name, const ls::Vec2I& center)
{
    const ls::Vec2I& size = params.defaultSize();
    const ls::Vec2I localCenter = [&]() {
        if (params.hasParentWindow())
        {
            const auto& parent = params.parentWindow();
            return center + parent.windowRect().min;
        }
        else return center;
    }();

    const ls::Vec2I topLeft = localCenter - size / 2;
    const ls::Rectangle2I rect(topLeft, topLeft + size);

    m_freeWindows.emplace_back(rect, name, params);
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

WindowSpaceManager::BackgroundWindowHandle WindowSpaceManager::Scene::findBackgroundWindow(const std::string& title)
{
    return m_backgroundWindows.findIf([&title](const BackgroundWindow& window)->bool {return window.title() == title; });
}
WindowSpaceManager::ConstBackgroundWindowHandle WindowSpaceManager::Scene::findBackgroundWindow(const std::string& title) const
{
    return m_backgroundWindows.findIf([&title](const BackgroundWindow& window)->bool {return window.title() == title; });
}
WindowSpaceManager::WindowFullLocalization WindowSpaceManager::Scene::fullLocalizationOf(BackgroundWindowHandle h)
{
    return { m_windowSpaceManager, &(h.data()) };
}

void WindowSpaceManager::Scene::update(const ls::Rectangle2I& rect)
{
    update(m_backgroundWindows.root(), rect);
}
void WindowSpaceManager::Scene::update(BackgroundWindowHandle h, const ls::Rectangle2I& rect)
{
    window(h).setWindowRect(rect);
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

        if (!ls::intersect(window(currentRegionHandle).windowRect(), pos))
        {
            return {};
        }

        if (!hasChildren(currentRegionHandle)) break;

        ConstBackgroundWindowHandle leftChildHandle = currentRegionHandle.left();
        ConstBackgroundWindowHandle rightChildHandle = currentRegionHandle.right();

        //explore further the intersecting region
        if (ls::intersect(window(leftChildHandle).windowRect(), pos))
        {
            currentRegionHandle = leftChildHandle;
        }
        else if (ls::intersect(window(rightChildHandle).windowRect(), pos))
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
        Window& wnd = window(h);
        wnd.draw(renderTarget, renderStates);
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
sf::View WindowSpaceManager::getWindowView(const WindowSpaceManager::Window& window, const ls::Rectangle2F& worldRect) const
{
    return getRectView(window.windowRect(), worldRect);
}
void WindowSpaceManager::setRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const
{
    m_window.setView(getRectView(viewport, worldRect));
}
void WindowSpaceManager::setWindowView(const WindowSpaceManager::Window& window, const ls::Rectangle2F& worldRect) const
{
    m_window.setView(getWindowView(window, worldRect));
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