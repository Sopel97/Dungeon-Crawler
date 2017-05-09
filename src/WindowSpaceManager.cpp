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
            return ls::Rectangle2I::withSize(space.min + ls::Vec2I(widthDiff / 2, 0), width, fullHeight);
    }

    //go with full width
    {
        const int fullWidth = space.width();
        const int height = fullWidth / m_ratio;
        const int heightDiff = space.height() - height;

        return ls::Rectangle2I::withSize(space.min + ls::Vec2I(0, heightDiff / 2), fullWidth, height);
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


WindowSpaceManager::WindowParams WindowSpaceManager::Window::defaultParams()
{
    WindowParams params;

    params.minWindowWidth = 0;
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = 0;

    params.maxWindowWidth = std::nullopt;
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = std::nullopt;

    params.isMinimizable = false;
    params.isCloseable = true;
    params.isResizeable = false;
    params.isMovable = false;
    params.isContentOnly = false;
    params.hasHeader = false;
    params.hasScrollBar = false;

    return params;
}
WindowSpaceManager::Window::Window(const ls::Rectangle2I& windowRect, const std::string& name) :
    Window(windowRect, name, defaultParams())
{

}
WindowSpaceManager::Window::Window(const ls::Rectangle2I& windowRect, const std::string& name, const WindowParams& params) :
    m_windowRect(windowRect),
    m_params(params),
    m_name(name),
    m_parent(nullptr),
    m_spaceUser(nullptr)
{

}

WindowSpaceManager::Window::~Window()
{

}

const ls::Rectangle2I& WindowSpaceManager::Window::windowRect() const
{
    return m_windowRect;
}
ls::Rectangle2I WindowSpaceManager::Window::absoluteWindowRect() const
{
    if (m_parent != nullptr) return m_windowRect.translated(m_parent->absoluteContentRect().min);

    return m_windowRect;
}
ls::Rectangle2I WindowSpaceManager::Window::absoluteContentRect() const
{
    if (m_params.isContentOnly) return absoluteWindowRect();

    ls::Rectangle2I rect = absoluteWindowRect();
    ls::Vec2I offsetFromTopLeft(m_windowLeftBarWidth, m_windowTopBarHeight);
    if (m_params.hasHeader)
    {
        offsetFromTopLeft.y = m_windowHeaderHeight;
    }

    ls::Vec2I offsetFromBottomRight(-m_windowRightBarWidth, -m_windowBottomBarHeight);
    if (m_params.hasScrollBar)
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
void WindowSpaceManager::Window::setWindowWidth(int newWidth)
{
    m_windowRect.max.x = m_windowRect.min.x + newWidth;
}
void WindowSpaceManager::Window::setWindowHeight(int newHeight)
{
    m_windowRect.max.y = m_windowRect.min.y + newHeight;
}
void WindowSpaceManager::Window::setContentSize(const ls::Vec2I& newSize)
{
    const auto currentContentRect = absoluteContentRect();
    // compute the padding
    const int widthDiff = m_windowRect.width() - currentContentRect.width();
    const int heightDiff = m_windowRect.height() - currentContentRect.height();
    setWindowSize(newSize + ls::Vec2I(widthDiff, heightDiff));
}
void WindowSpaceManager::Window::setContentWidth(int newWidth)
{
    const auto currentContentRect = absoluteContentRect();
    const int widthDiff = m_windowRect.width() - currentContentRect.width();
    setWindowWidth(newWidth + widthDiff);
}
void WindowSpaceManager::Window::setContentHeight(int newHeight)
{
    const auto currentContentRect = absoluteContentRect();
    const int heightDiff = m_windowRect.height() - currentContentRect.height();
    setWindowHeight(newHeight + heightDiff);
}

int WindowSpaceManager::Window::minWindowWidth() const
{
    return m_params.minWindowWidth;
}
int WindowSpaceManager::Window::minWindowHeight() const
{
    return m_params.minWindowHeight;
}
int WindowSpaceManager::Window::minContentWidth() const
{
    return m_params.minContentWidth;
}
int WindowSpaceManager::Window::minContentHeight() const
{
    return m_params.minContentHeight;
}
bool WindowSpaceManager::Window::hasMaxWindowWidth() const
{
    return m_params.maxWindowWidth.has_value();
}
bool WindowSpaceManager::Window::hasMaxWindowHeight() const
{
    return m_params.maxWindowHeight.has_value();
}
bool WindowSpaceManager::Window::hasMaxContentWidth() const
{
    return m_params.maxContentWidth.has_value();
}
bool WindowSpaceManager::Window::hasMaxContentHeight() const
{
    return m_params.maxContentHeight.has_value();
}
int WindowSpaceManager::Window::maxWindowWidth() const
{
    return m_params.maxWindowWidth.value();
}
int WindowSpaceManager::Window::maxWindowHeight() const
{
    return m_params.maxWindowHeight.value();
}
int WindowSpaceManager::Window::maxContentWidth() const
{
    return m_params.maxContentWidth.value();
}
int WindowSpaceManager::Window::maxContentHeight() const
{
    return m_params.maxContentHeight.value();
}

void WindowSpaceManager::Window::setMinWindowWidth(int val) 
{
    m_params.minWindowWidth = val;
}
void WindowSpaceManager::Window::setMinWindowHeight(int val)
{
    m_params.minWindowHeight = val;
}
void WindowSpaceManager::Window::setMinContentWidth(int val)
{
    m_params.minContentWidth = val;
}
void WindowSpaceManager::Window::setMinContentHeight(int val)
{
    m_params.minContentHeight = val;
}
void WindowSpaceManager::Window::setMaxWindowWidth(int val)
{
    m_params.maxWindowWidth = val;
}
void WindowSpaceManager::Window::setMaxWindowHeight(int val)
{
    m_params.maxWindowHeight = val;
}
void WindowSpaceManager::Window::setMaxContentWidth(int val)
{
    m_params.maxContentWidth = val;
}
void WindowSpaceManager::Window::setMaxContentHeight(int val)
{
    m_params.maxContentHeight = val;
}

int WindowSpaceManager::Window::verticalScroll() const
{
    return 0;
}

bool WindowSpaceManager::Window::isMinimizable() const
{
    return m_params.isMinimizable;
}
bool WindowSpaceManager::Window::isCloseable() const
{
    return m_params.isCloseable;
}
bool WindowSpaceManager::Window::isResizeable() const
{
    return m_params.isResizeable;
}
bool WindowSpaceManager::Window::isMovable() const
{
    return m_params.isMovable;
}
bool WindowSpaceManager::Window::isContentOnly() const
{
    return m_params.isContentOnly;
}
bool WindowSpaceManager::Window::hasHeader() const
{
    return m_params.hasHeader;
}
bool WindowSpaceManager::Window::hasScrollBar() const
{
    return m_params.hasScrollBar;
}

void WindowSpaceManager::Window::setMinimizable(bool val)
{
    m_params.isMinimizable = val;
}
void WindowSpaceManager::Window::setCloseable(bool val)
{
    m_params.isCloseable = val;
}
void WindowSpaceManager::Window::setResizeable(bool val)
{
    m_params.isResizeable = val;
}
void WindowSpaceManager::Window::setMovable(bool val)
{
    m_params.isMovable = val;
}
void WindowSpaceManager::Window::setContentOnly(bool val)
{
    m_params.isContentOnly = val;
}
void WindowSpaceManager::Window::setHeaderEnabled(bool val)
{
    m_params.hasHeader = val;
}
void WindowSpaceManager::Window::setScrollBarEnabled(bool val)
{
    m_params.hasScrollBar = val;
}

bool WindowSpaceManager::Window::hasEventHandler() const
{
    return m_spaceUser != nullptr;
}
SfmlEventHandler& WindowSpaceManager::Window::eventHandler()
{
    return *m_spaceUser;
}
void WindowSpaceManager::Window::setParent(Window& parent)
{
    m_parent = &parent;
}
bool WindowSpaceManager::Window::hasParent() const
{
    return m_parent != nullptr;
}
void WindowSpaceManager::Window::removeParent()
{
    m_parent = nullptr;
}

void WindowSpaceManager::Window::setUser(WindowSpaceUser& newUser)
{
    if (m_spaceUser != nullptr) m_spaceUser->detach();

    m_spaceUser = &newUser;
}
void WindowSpaceManager::Window::removeUser()
{
    m_spaceUser = nullptr;
}
WindowSpaceUser* WindowSpaceManager::Window::user()
{
    return m_spaceUser;
}

ls::Vec2I WindowSpaceManager::Window::localWindowCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - absoluteWindowRect().min;
}
ls::Vec2I WindowSpaceManager::Window::localContentCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - this->absoluteContentRect().min;
}

void WindowSpaceManager::Window::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    if (m_spaceUser != nullptr) m_spaceUser->draw(renderTarget, renderStates);
}


WindowSpaceManager::WindowParams WindowSpaceManager::BackgroundWindow::defaultParams()
{
    WindowParams params;

    params.minWindowWidth = 0;
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = 0;

    params.maxWindowWidth = std::nullopt;
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = std::nullopt;

    params.isMinimizable = false;
    params.isCloseable = false;
    params.isResizeable = false;
    params.isMovable = false;
    params.isContentOnly = true;
    params.hasHeader = false;
    params.hasScrollBar = false;

    return params;
}

WindowSpaceManager::WindowParams WindowSpaceManager::FreeWindow::defaultParams()
{
    WindowParams params;

    params.minWindowWidth = 0;
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = 0;

    params.maxWindowWidth = std::nullopt;
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = std::nullopt;

    params.isMinimizable = false;
    params.isCloseable = true;
    params.isResizeable = false;
    params.isMovable = false;
    params.isContentOnly = false;
    params.hasHeader = true;
    params.hasScrollBar = false;

    return params;
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
    const ls::Rectangle2I& rect = window(h).absoluteContentRect();
    setSubdivisionParams(h, params);
    // they have to be translated to local coordinates, so (0,0) is the top left
    auto subdividedRects = params.calculateSubRects(rect.translated(-rect.min));

    BackgroundWindowHandle left = m_backgroundWindows.emplaceLeft(h, subdividedRects.first, nameFirst, window(h));
    BackgroundWindowHandle right = m_backgroundWindows.emplaceRight(h, subdividedRects.second, nameSecond, window(h));

    auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
    *it = left;	// it is guaranteed to exist
    m_topmostRegions.push_back(right);

    return { left, right };
}
WindowSpaceManager::FreeWindowHandle WindowSpaceManager::Scene::createFreeWindow(const std::string& name, const ls::Vec2I& center, const ls::Vec2I& size)
{
    const ls::Vec2I topLeft = center - size / 2;
    const ls::Rectangle2I rect(topLeft, topLeft + size);

    m_freeWindows.emplace_back(rect, name);
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

    // they have to be translated to local coordinates, so (0,0) is the top left
    auto subdividedRects = subdivisionParams(h).calculateSubRects(rect.translated(-rect.min));
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
    return getRectView(window.absoluteWindowRect(), worldRect);
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