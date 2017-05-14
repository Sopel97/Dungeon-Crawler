#include "window/InternalWindow.h"

#include "ResourceManager.h"
#include "SfmlEventHandler.h"
#include "window/WindowContent.h"
#include "window/WindowSpaceManager.h"

using namespace ls;

const int InternalWindow::m_windowTopBarHeight = 4;
const int InternalWindow::m_windowHeaderHeight = 15;
const int InternalWindow::m_windowLeftBarWidth = 4;
const int InternalWindow::m_windowBottomBarHeight = 4;
const int InternalWindow::m_windowRightBarWidth = 4;
const int InternalWindow::m_windowScrollBarWidth = 12;
const Vec2I InternalWindow::m_windowHighTopLeftCornerSize = { m_windowLeftBarWidth, m_windowHeaderHeight };
const Vec2I InternalWindow::m_windowLowTopLeftCornerSize = { m_windowLeftBarWidth, m_windowTopBarHeight };
const Vec2I InternalWindow::m_windowHighTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I InternalWindow::m_windowLowTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I InternalWindow::m_windowBottomLeftCornerSize = { m_windowLeftBarWidth, m_windowBottomBarHeight };
const Vec2I InternalWindow::m_windowBottomRightCornerSize = { m_windowRightBarWidth, m_windowBottomBarHeight };
const Vec2I InternalWindow::m_windowButtonSize = { 12, 12 };
const Vec2I InternalWindow::m_windowScrollSliderSize = { 12, 12 };

const Vec2I InternalWindow::m_windowTopBarSpritePosition = { 0, 0 };
const Vec2I InternalWindow::m_windowHeaderSpritePosition = { 0, 8 };
const Vec2I InternalWindow::m_windowLeftBarSpritePosition = { 0, 0 };
const Vec2I InternalWindow::m_windowRightBarSpritePosition = { 4, 0 };
const Vec2I InternalWindow::m_windowBottomBarSpritePosition = { 0, 4 };
const Vec2I InternalWindow::m_windowHighTopLeftCornerSpritePosition = { 206, 19 };
const Vec2I InternalWindow::m_windowLowTopLeftCornerSpritePosition = { 198, 19 };
const Vec2I InternalWindow::m_windowHighTopRightCornerSpritePosition = { 210, 19 };
const Vec2I InternalWindow::m_windowLowTopRightCornerSpritePosition = { 202, 19 };
const Vec2I InternalWindow::m_windowBottomLeftCornerSpritePosition = { 198, 23 };
const Vec2I InternalWindow::m_windowBottomRightCornerSpritePosition = { 202, 23 };
const Vec2I InternalWindow::m_windowScrollBarSpritePosition = { 8, 0 };
const Vec2I InternalWindow::m_windowScrollBarUpButtonSpritePosition = { 0, 57 };
const Vec2I InternalWindow::m_windowScrollBarDownButtonSpritePosition = { 12, 57 };
const Vec2I InternalWindow::m_windowScrollBarSliderSpritePosition = { 24, 57 };


WindowParams InternalWindow::defaultParams()
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
InternalWindow::InternalWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name) :
    InternalWindow(wsm, windowRect, name, defaultParams())
{

}
InternalWindow::InternalWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& windowRect, const std::string& name, const WindowParams& params) :
    m_windowRect(windowRect),
    m_params(params),
    m_name(name),
    m_wsm(&wsm),
    m_parent(nullptr),
    m_content(nullptr),
    m_isMinimized(false)
{

}

InternalWindow::~InternalWindow()
{

}

ls::Rectangle2I InternalWindow::windowRect() const
{
    if (m_isMinimized)
    {
        return ls::Rectangle2I::withSize(m_windowRect.min, m_windowRect.width(), m_windowHeaderHeight);
    }

    return m_windowRect;
}
ls::Rectangle2I InternalWindow::absoluteWindowRect() const
{
    if (m_parent != nullptr) return m_windowRect.translated(m_parent->absoluteContentRect().min);

    return windowRect();
}
ls::Rectangle2I InternalWindow::absoluteContentRect() const
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
const std::string& InternalWindow::title() const
{
    return m_name;
}

void InternalWindow::setWindowRect(const ls::Rectangle2I& newRect)
{
    m_windowRect = newRect;
}
void InternalWindow::setWindowPosition(const ls::Vec2I& newPosition)
{
    m_windowRect.translate(newPosition - m_windowRect.min);
}
void InternalWindow::setWindowSize(const ls::Vec2I& newSize)
{
    m_windowRect.max = m_windowRect.min + newSize;
}
void InternalWindow::setWindowWidth(int newWidth)
{
    m_windowRect.max.x = m_windowRect.min.x + newWidth;
}
void InternalWindow::setWindowHeight(int newHeight)
{
    m_windowRect.max.y = m_windowRect.min.y + newHeight;
}
void InternalWindow::setContentSize(const ls::Vec2I& newSize)
{
    const auto currentContentRect = absoluteContentRect();
    // compute the padding
    const int widthDiff = m_windowRect.width() - currentContentRect.width();
    const int heightDiff = m_windowRect.height() - currentContentRect.height();
    setWindowSize(newSize + ls::Vec2I(widthDiff, heightDiff));
}
void InternalWindow::setContentWidth(int newWidth)
{
    const auto currentContentRect = absoluteContentRect();
    const int widthDiff = m_windowRect.width() - currentContentRect.width();
    setWindowWidth(newWidth + widthDiff);
}
void InternalWindow::setContentHeight(int newHeight)
{
    const auto currentContentRect = absoluteContentRect();
    const int heightDiff = m_windowRect.height() - currentContentRect.height();
    setWindowHeight(newHeight + heightDiff);
}

int InternalWindow::minWindowWidth() const
{
    return m_params.minWindowWidth;
}
int InternalWindow::minWindowHeight() const
{
    return m_params.minWindowHeight;
}
int InternalWindow::minContentWidth() const
{
    return m_params.minContentWidth;
}
int InternalWindow::minContentHeight() const
{
    return m_params.minContentHeight;
}
bool InternalWindow::hasMaxWindowWidth() const
{
    return m_params.maxWindowWidth.has_value();
}
bool InternalWindow::hasMaxWindowHeight() const
{
    return m_params.maxWindowHeight.has_value();
}
bool InternalWindow::hasMaxContentWidth() const
{
    return m_params.maxContentWidth.has_value();
}
bool InternalWindow::hasMaxContentHeight() const
{
    return m_params.maxContentHeight.has_value();
}
int InternalWindow::maxWindowWidth() const
{
    return m_params.maxWindowWidth.value();
}
int InternalWindow::maxWindowHeight() const
{
    return m_params.maxWindowHeight.value();
}
int InternalWindow::maxContentWidth() const
{
    return m_params.maxContentWidth.value();
}
int InternalWindow::maxContentHeight() const
{
    return m_params.maxContentHeight.value();
}

void InternalWindow::setMinWindowWidth(int val)
{
    m_params.minWindowWidth = val;
}
void InternalWindow::setMinWindowHeight(int val)
{
    m_params.minWindowHeight = val;
}
void InternalWindow::setMinContentWidth(int val)
{
    m_params.minContentWidth = val;
}
void InternalWindow::setMinContentHeight(int val)
{
    m_params.minContentHeight = val;
}
void InternalWindow::setMaxWindowWidth(int val)
{
    m_params.maxWindowWidth = val;
}
void InternalWindow::setMaxWindowHeight(int val)
{
    m_params.maxWindowHeight = val;
}
void InternalWindow::setMaxContentWidth(int val)
{
    m_params.maxContentWidth = val;
}
void InternalWindow::setMaxContentHeight(int val)
{
    m_params.maxContentHeight = val;
}

int InternalWindow::verticalScroll() const
{
    return 0;
}

bool InternalWindow::isMinimizable() const
{
    return m_params.isMinimizable;
}
bool InternalWindow::isCloseable() const
{
    return m_params.isCloseable;
}
bool InternalWindow::isResizeable() const
{
    return m_params.isResizeable;
}
bool InternalWindow::isMovable() const
{
    return m_params.isMovable;
}
bool InternalWindow::isContentOnly() const
{
    return m_params.isContentOnly;
}
bool InternalWindow::hasHeader() const
{
    return m_params.hasHeader;
}
bool InternalWindow::hasScrollBar() const
{
    return m_params.hasScrollBar;
}
bool InternalWindow::isMinimized() const
{
    return m_isMinimized;
}

void InternalWindow::setMinimizable(bool val)
{
    m_params.isMinimizable = val;
}
void InternalWindow::setCloseable(bool val)
{
    m_params.isCloseable = val;
}
void InternalWindow::setResizeable(bool val)
{
    m_params.isResizeable = val;
}
void InternalWindow::setMovable(bool val)
{
    m_params.isMovable = val;
}
void InternalWindow::setContentOnly(bool val)
{
    m_params.isContentOnly = val;
}
void InternalWindow::setHeaderEnabled(bool val)
{
    m_params.hasHeader = val;
}
void InternalWindow::setScrollBarEnabled(bool val)
{
    m_params.hasScrollBar = val;
}

SfmlEventHandler::EventResult InternalWindow::onTextEntered(sf::Event::TextEvent& event, EventContext context)
{
    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onTextEntered(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onKeyPressed(sf::Event::KeyEvent& event, EventContext context)
{
    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onKeyPressed(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onKeyReleased(sf::Event::KeyEvent& event, EventContext context)
{
    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onKeyReleased(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context)
{
    const ls::Vec2I mousePos{ event.x, event.y };
    if (!ls::intersect(mousePos, absoluteContentRect())) return { false, false };

    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onMouseWheelMoved(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context)
{
    const ls::Vec2I mousePos{ event.x, event.y };
    if (!ls::intersect(mousePos, absoluteContentRect())) return { false, false };

    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onMouseButtonPressed(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context)
{
    const ls::Vec2I mousePos{ event.x, event.y };
    if (!ls::intersect(mousePos, absoluteContentRect())) return { false, false };

    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onMouseButtonReleased(event, context);
}
SfmlEventHandler::EventResult InternalWindow::onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context)
{
    const ls::Vec2I mousePos{ event.x, event.y };
    if (!ls::intersect(mousePos, absoluteContentRect())) return { false, false };

    if (m_content == nullptr || m_isMinimized) return { false, false };
    return m_content->onMouseMoved(event, context);
}

void InternalWindow::setParent(InternalWindow& parent)
{
    m_parent = &parent;
}
bool InternalWindow::hasParent() const
{
    return m_parent != nullptr;
}
void InternalWindow::removeParent()
{
    m_parent = nullptr;
}

void InternalWindow::setContent(WindowContent& newUser)
{
    if (m_content != nullptr) m_content->detach();

    m_content = &newUser;
}
void InternalWindow::removeContent()
{
    m_content = nullptr;
}
WindowSpaceManager& InternalWindow::windowSpaceManager()
{
    return *m_wsm;
}
const WindowSpaceManager& InternalWindow::windowSpaceManager() const
{
    return *m_wsm;
}

ls::Vec2I InternalWindow::localWindowCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - absoluteWindowRect().min;
}
ls::Vec2I InternalWindow::localContentCoords(const ls::Vec2I& globalCoords) const
{
    return globalCoords - this->absoluteContentRect().min;
}

void InternalWindow::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    if (m_isMinimized)
    {
        drawSkeletonMinimized(renderTarget, renderStates);
        return;
    }
    else
    {
        drawSkeleton(renderTarget, renderStates);
    }

    if (m_content != nullptr)
    {
        const auto contentRect = absoluteContentRect();
        m_wsm->setRectView(contentRect, Rectangle2F::withSize(Vec2F(0.0f, static_cast<float>(this->verticalScroll())), static_cast<float>(contentRect.width()), static_cast<float>(contentRect.height())));
        m_content->draw(renderTarget, renderStates);
    }
}
void InternalWindow::drawSkeleton(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    if (isContentOnly()) return;

    ResourceHandle<sf::Texture> backgroundTexture = ResourceManager::instance().get<sf::Texture>("UiBackground");
    ResourceHandle<sf::Texture> verticalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiVerticalBars");
    ResourceHandle<sf::Texture> horizontalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiHorizontalBars");
    ResourceHandle<sf::Texture> nonRepeatingSprites = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    const auto windowRect = absoluteWindowRect();
    const auto contentRect = absoluteContentRect();

    m_wsm->setDefaultView();

    const int& topBarHeight = hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    const Vec2I& topBarSpritePosition = hasHeader() ? m_windowHeaderSpritePosition : m_windowTopBarSpritePosition;
    const Vec2I& topLeftCornerSpritePosition = hasHeader() ? m_windowHighTopLeftCornerSpritePosition : m_windowLowTopLeftCornerSpritePosition;
    const Vec2I& topRightCornerSpritePosition = hasHeader() ? m_windowHighTopRightCornerSpritePosition : m_windowLowTopRightCornerSpritePosition;

    const Vec2I& windowTopLeft = windowRect.min;
    const Vec2I& windowBottomRight = windowRect.max;
    const Vec2I& windowInteriorTopLeft = contentRect.min;

    int windowWidth = windowRect.width();
    int windowHeight = windowRect.height();
    int windowInteriorWidth = windowRect.width() - (m_windowLeftBarWidth + m_windowRightBarWidth);
    int windowInteriorHeight = windowRect.height() - (m_windowBottomBarHeight + topBarHeight);
    int windowContentWidth = windowInteriorWidth;
    if (hasScrollBar()) windowContentWidth -= m_windowScrollBarWidth;
    int windowContentHeight = windowInteriorHeight;

    const Vec2I& windowInteriorBottomRight = windowInteriorTopLeft + ls::Vec2I(windowInteriorWidth, windowInteriorHeight);

    sf::Sprite topBarSprite;
    topBarSprite.setPosition(static_cast<float>(windowInteriorTopLeft.x), static_cast<float>(windowTopLeft.y));
    topBarSprite.setTexture(horizontalBarsSprites.get());
    topBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(topBarSpritePosition.x, topBarSpritePosition.y), sf::Vector2i(windowInteriorWidth, topBarHeight)));
    renderTarget.draw(topBarSprite, renderStates);

    sf::Sprite bottomBarSprite;
    bottomBarSprite.setPosition(static_cast<float>(windowInteriorTopLeft.x), static_cast<float>(windowInteriorBottomRight.y));
    bottomBarSprite.setTexture(horizontalBarsSprites.get());
    bottomBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowBottomBarSpritePosition.x, m_windowBottomBarSpritePosition.y), sf::Vector2i(windowInteriorWidth, m_windowBottomBarHeight)));
    renderTarget.draw(bottomBarSprite, renderStates);

    sf::Sprite leftBarSprite;
    leftBarSprite.setPosition(static_cast<float>(windowTopLeft.x), static_cast<float>(windowInteriorTopLeft.y));
    leftBarSprite.setTexture(verticalBarsSprites.get());
    leftBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowLeftBarSpritePosition.x, m_windowLeftBarSpritePosition.y), sf::Vector2i(m_windowLeftBarWidth, windowInteriorHeight)));
    renderTarget.draw(leftBarSprite, renderStates);

    sf::Sprite rightBarSprite;
    rightBarSprite.setPosition(static_cast<float>(windowInteriorBottomRight.x), static_cast<float>(windowInteriorTopLeft.y));
    rightBarSprite.setTexture(verticalBarsSprites.get());
    rightBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowRightBarSpritePosition.x, m_windowRightBarSpritePosition.y), sf::Vector2i(m_windowRightBarWidth, windowInteriorHeight)));
    renderTarget.draw(rightBarSprite, renderStates);

    sf::Sprite topLeftCornerSprite;
    topLeftCornerSprite.setPosition(static_cast<float>(windowTopLeft.x), static_cast<float>(windowTopLeft.y));
    topLeftCornerSprite.setTexture(nonRepeatingSprites.get());
    topLeftCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(topLeftCornerSpritePosition.x, topLeftCornerSpritePosition.y), sf::Vector2i(m_windowHighTopLeftCornerSize.x, topBarHeight)));
    renderTarget.draw(topLeftCornerSprite, renderStates);

    sf::Sprite topRightCornerSprite;
    topRightCornerSprite.setPosition(static_cast<float>(windowInteriorBottomRight.x), static_cast<float>(windowTopLeft.y));
    topRightCornerSprite.setTexture(nonRepeatingSprites.get());
    topRightCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(topRightCornerSpritePosition.x, topRightCornerSpritePosition.y), sf::Vector2i(m_windowHighTopRightCornerSize.x, topBarHeight)));
    renderTarget.draw(topRightCornerSprite, renderStates);

    sf::Sprite bottomLeftCornerSprite;
    bottomLeftCornerSprite.setPosition(static_cast<float>(windowTopLeft.x), static_cast<float>(windowInteriorBottomRight.y));
    bottomLeftCornerSprite.setTexture(nonRepeatingSprites.get());
    bottomLeftCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowBottomLeftCornerSpritePosition.x, m_windowBottomLeftCornerSpritePosition.y), sf::Vector2i(m_windowBottomLeftCornerSize.x, m_windowBottomLeftCornerSize.y)));
    renderTarget.draw(bottomLeftCornerSprite, renderStates);

    sf::Sprite bottomRightCornerSprite;
    bottomRightCornerSprite.setPosition(static_cast<float>(windowInteriorBottomRight.x), static_cast<float>(windowInteriorBottomRight.y));
    bottomRightCornerSprite.setTexture(nonRepeatingSprites.get());
    bottomRightCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowBottomRightCornerSpritePosition.x, m_windowBottomRightCornerSpritePosition.y), sf::Vector2i(m_windowBottomRightCornerSize.x, m_windowBottomRightCornerSize.y)));
    renderTarget.draw(bottomRightCornerSprite, renderStates);

    sf::Sprite backgroundSprite;
    backgroundSprite.setPosition(static_cast<float>(windowInteriorTopLeft.x), static_cast<float>(windowInteriorTopLeft.y));
    backgroundSprite.setTexture(backgroundTexture.get());
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, this->verticalScroll()), sf::Vector2i(windowContentWidth, windowContentHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    if (hasScrollBar())
    {
        int scrollBarHeight = windowInteriorHeight;

        const Vec2I scrollBarTopLeft(contentRect.min.x + windowContentWidth, contentRect.min.y);

        sf::Sprite scrollBarSprite;
        scrollBarSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(scrollBarTopLeft.y));
        scrollBarSprite.setTexture(verticalBarsSprites.get());
        scrollBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarSpritePosition.x, m_windowScrollBarSpritePosition.y), sf::Vector2i(m_windowScrollBarWidth, scrollBarHeight)));
        renderTarget.draw(scrollBarSprite, renderStates);

        sf::Sprite scrollBarUpButtonSprite;
        scrollBarUpButtonSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(scrollBarTopLeft.y));
        scrollBarUpButtonSprite.setTexture(nonRepeatingSprites.get());
        scrollBarUpButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarUpButtonSpritePosition.x, m_windowScrollBarUpButtonSpritePosition.y), sf::Vector2i(m_windowButtonSize.x, m_windowButtonSize.y)));
        renderTarget.draw(scrollBarUpButtonSprite, renderStates);

        sf::Sprite scrollBarDownButtonSprite;
        scrollBarDownButtonSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(scrollBarTopLeft.y + scrollBarHeight - m_windowButtonSize.y));
        scrollBarDownButtonSprite.setTexture(nonRepeatingSprites.get());
        scrollBarDownButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarDownButtonSpritePosition.x, m_windowScrollBarDownButtonSpritePosition.y), sf::Vector2i(m_windowButtonSize.x, m_windowButtonSize.y)));
        renderTarget.draw(scrollBarDownButtonSprite, renderStates);

        if (windowContentHeight < contentRect.height())
        {
            int sliderMin = static_cast<int>(scrollBarTopLeft.y) + m_windowButtonSize.y / 2 + m_windowButtonSize.y;
            int sliderMax = static_cast<int>(scrollBarTopLeft.y) + scrollBarHeight - m_windowButtonSize.y / 2 - 2 * m_windowButtonSize.y;
            int sliderRangeLength = sliderMax - sliderMin;
            int scrollRangeLength = contentRect.height() - windowContentHeight;
            float scrolled = static_cast<float>(this->verticalScroll()) / static_cast<float>(scrollRangeLength);
            int sliderPosition = static_cast<int>(sliderMin + sliderRangeLength * scrolled);

            sf::Sprite scrollBarSliderButtonSprite;
            scrollBarSliderButtonSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(sliderPosition - m_windowButtonSize.y / 2));
            scrollBarSliderButtonSprite.setTexture(nonRepeatingSprites.get());
            scrollBarSliderButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarSliderSpritePosition.x, m_windowScrollBarSliderSpritePosition.y), sf::Vector2i(m_windowButtonSize.x, m_windowButtonSize.y)));
            renderTarget.draw(scrollBarSliderButtonSprite, renderStates);
        }
    }
}

void InternalWindow::drawSkeletonMinimized(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    ResourceHandle<sf::Texture> horizontalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiHorizontalBars");
    ResourceHandle<sf::Texture> nonRepeatingSprites = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    const auto windowRect = absoluteWindowRect();

    m_wsm->setDefaultView();

    const int& topBarHeight = m_windowHeaderHeight;
    const Vec2I& topBarSpritePosition = m_windowHeaderSpritePosition;
    const Vec2I& topLeftCornerSpritePosition = m_windowHighTopLeftCornerSpritePosition;
    const Vec2I& topRightCornerSpritePosition = m_windowHighTopRightCornerSpritePosition;

    const Vec2I& windowTopLeft = windowRect.min;
    const Vec2I& windowBottomRight = windowRect.max;

    int windowWidth = windowRect.width();
    int windowHeight = windowRect.height();
    sf::Sprite topBarSprite;
    topBarSprite.setPosition(static_cast<float>(windowTopLeft.x + m_windowLeftBarWidth), static_cast<float>(windowTopLeft.y));
    topBarSprite.setTexture(horizontalBarsSprites.get());
    topBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(topBarSpritePosition.x, topBarSpritePosition.y), sf::Vector2i(windowWidth - (m_windowLeftBarWidth + m_windowRightBarWidth), topBarHeight)));
    renderTarget.draw(topBarSprite, renderStates);

    sf::Sprite topLeftCornerSprite;
    topLeftCornerSprite.setPosition(static_cast<float>(windowTopLeft.x), static_cast<float>(windowTopLeft.y));
    topLeftCornerSprite.setTexture(nonRepeatingSprites.get());
    topLeftCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(topLeftCornerSpritePosition.x, topLeftCornerSpritePosition.y), sf::Vector2i(m_windowHighTopLeftCornerSize.x, topBarHeight)));
    renderTarget.draw(topLeftCornerSprite, renderStates);

    sf::Sprite topRightCornerSprite;
    topRightCornerSprite.setPosition(static_cast<float>(windowBottomRight.x - m_windowRightBarWidth), static_cast<float>(windowTopLeft.y));
    topRightCornerSprite.setTexture(nonRepeatingSprites.get());
    topRightCornerSprite.setTextureRect(sf::IntRect(sf::Vector2i(topRightCornerSpritePosition.x, topRightCornerSpritePosition.y), sf::Vector2i(m_windowHighTopRightCornerSize.x, topBarHeight)));
    renderTarget.draw(topRightCornerSprite, renderStates);
}
