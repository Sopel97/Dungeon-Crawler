#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Player.h"
#include "Root.h"
#include "WindowSpaceManager.h"

#include <algorithm>

using namespace ls;

const int PlayerUi::m_playerUiPanelWidth = 230;
const int PlayerUi::m_windowTopBarHeight = 4;
const int PlayerUi::m_windowHeaderHeight = 15;
const int PlayerUi::m_windowLeftBarWidth = 4;
const int PlayerUi::m_windowBottomBarHeight = 4;
const int PlayerUi::m_windowRightBarWidth = 4;
const int PlayerUi::m_windowScrollBarWidth = 12;
const Vec2I PlayerUi::m_windowHighTopLeftCornerSize = { m_windowLeftBarWidth, m_windowHeaderHeight };
const Vec2I PlayerUi::m_windowLowTopLeftCornerSize = { m_windowLeftBarWidth, m_windowTopBarHeight };
const Vec2I PlayerUi::m_windowHighTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I PlayerUi::m_windowLowTopRightCornerSize = { m_windowRightBarWidth, m_windowTopBarHeight };
const Vec2I PlayerUi::m_windowBottomLeftCornerSize = { m_windowLeftBarWidth, m_windowBottomBarHeight };
const Vec2I PlayerUi::m_windowBottomRightCornerSize = { m_windowRightBarWidth, m_windowBottomBarHeight };
const Vec2I PlayerUi::m_windowButtonSize = { 12, 12 };
const Vec2I PlayerUi::m_windowScrollSliderSize = { 12, 12 };

const Vec2I PlayerUi::m_windowTopBarSpritePosition = { 0, 0 };
const Vec2I PlayerUi::m_windowHeaderSpritePosition = { 0, 8 };
const Vec2I PlayerUi::m_windowLeftBarSpritePosition = { 0, 0 };
const Vec2I PlayerUi::m_windowRightBarSpritePosition = { 4, 0 };
const Vec2I PlayerUi::m_windowBottomBarSpritePosition = { 0, 4 };
const Vec2I PlayerUi::m_windowHighTopLeftCornerSpritePosition = { 206, 19 };
const Vec2I PlayerUi::m_windowLowTopLeftCornerSpritePosition = { 198, 19 };
const Vec2I PlayerUi::m_windowHighTopRightCornerSpritePosition = { 210, 19 };
const Vec2I PlayerUi::m_windowLowTopRightCornerSpritePosition = { 202, 19 };
const Vec2I PlayerUi::m_windowBottomLeftCornerSpritePosition = { 198, 23 };
const Vec2I PlayerUi::m_windowBottomRightCornerSpritePosition = { 202, 23 };
const Vec2I PlayerUi::m_windowScrollBarSpritePosition = { 8, 0 };
const Vec2I PlayerUi::m_windowScrollBarUpButtonSpritePosition = { 0, 57 };
const Vec2I PlayerUi::m_windowScrollBarDownButtonSpritePosition = { 12, 57 };
const Vec2I PlayerUi::m_windowScrollBarSliderSpritePosition = { 24, 57 };

PlayerUi::PlayerUi(Player& player, const WindowSpaceManager::WindowFullLocalization& loc) :
	WindowSpaceUser(loc),
    m_player(player)
{
}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for(auto wnd : m_windows)
    {
		drawWindow(renderTarget, renderStates, *wnd);
		setContentViewOfWindow(*wnd);
        wnd->draw(renderTarget, renderStates);
    }
}

void PlayerUi::drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const PanelWindow& wnd)
{
    const Rectangle2I& playerUiPanelRect = window().absoluteWindowRect();
    const int& topBarHeight = wnd.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    Rectangle2I windowRect = wnd.absoluteWindowRect();
    Rectangle2I contentRect = wnd.absoluteContentRect();

    drawWindow(renderTarget, renderStates, wnd, windowRect, contentRect);
}
void PlayerUi::drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const PanelWindow& wnd, const Rectangle2I& windowRect, const Rectangle2I& contentRect)
{
    ResourceHandle<sf::Texture> backgroundTexture = ResourceManager::instance().get<sf::Texture>("UiBackground");
	ResourceHandle<sf::Texture> verticalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiVerticalBars");
    ResourceHandle<sf::Texture> horizontalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiHorizontalBars");
    ResourceHandle<sf::Texture> nonRepeatingSprites = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    windowSpaceManager().setDefaultView();

    const int& topBarHeight = wnd.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    const Vec2I& topBarSpritePosition = wnd.hasHeader() ? m_windowHeaderSpritePosition : m_windowTopBarSpritePosition;
    const Vec2I& topLeftCornerSpritePosition = wnd.hasHeader() ? m_windowHighTopLeftCornerSpritePosition : m_windowLowTopLeftCornerSpritePosition;
    const Vec2I& topRightCornerSpritePosition = wnd.hasHeader() ? m_windowHighTopRightCornerSpritePosition : m_windowLowTopRightCornerSpritePosition;

    const Vec2I& windowTopLeft = windowRect.min;
    const Vec2I& windowBottomRight = windowRect.max;
    const Vec2I& windowInteriorTopLeft = contentRect.min;

    int windowWidth = windowRect.width();
    int windowHeight = windowRect.height();
    int windowInteriorWidth = windowRect.width() - (m_windowLeftBarWidth + m_windowRightBarWidth);
    int windowInteriorHeight = windowRect.height() - (m_windowBottomBarHeight + topBarHeight);
    int windowContentWidth = windowInteriorWidth;
    if (wnd.hasScrollBar()) windowContentWidth -= m_windowScrollBarWidth;
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
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, wnd.verticalScroll()), sf::Vector2i(windowContentWidth, windowContentHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    if(wnd.hasScrollBar())
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

        auto contentRect = wnd.absoluteContentRect();
        if(windowContentHeight < contentRect.height())
        {
            int sliderMin = static_cast<int>(scrollBarTopLeft.y) + m_windowButtonSize.y / 2 + m_windowButtonSize.y;
            int sliderMax = static_cast<int>(scrollBarTopLeft.y) + scrollBarHeight - m_windowButtonSize.y / 2 - 2 * m_windowButtonSize.y;
            int sliderRangeLength = sliderMax - sliderMin;
            int scrollRangeLength = contentRect.height() - windowContentHeight;
            float scrolled = static_cast<float>(wnd.verticalScroll()) / static_cast<float>(scrollRangeLength);
            int sliderPosition = static_cast<int>(sliderMin + sliderRangeLength * scrolled);

            sf::Sprite scrollBarSliderButtonSprite;
            scrollBarSliderButtonSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(sliderPosition - m_windowButtonSize.y / 2));
            scrollBarSliderButtonSprite.setTexture(nonRepeatingSprites.get());
            scrollBarSliderButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarSliderSpritePosition.x, m_windowScrollBarSliderSpritePosition.y), sf::Vector2i(m_windowButtonSize.x, m_windowButtonSize.y)));
            renderTarget.draw(scrollBarSliderButtonSprite, renderStates);
        }
    }
}

void PlayerUi::setContentViewOfWindow(PanelWindow& wnd)
{
    const Rectangle2I& playerUiPanelRect = window().absoluteWindowRect();

    auto contentRect = wnd.absoluteContentRect();
    const Rectangle2I windowInteriorRect = contentRect;

    windowSpaceManager().setRectView(windowInteriorRect, Rectangle2F::withSize(Vec2F(0.0f, static_cast<float>(wnd.verticalScroll())), static_cast<float>(contentRect.width()), static_cast<float>(contentRect.height())));
}

void PlayerUi::closeWindow(PanelWindow* window)
{
    m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());

	updateWindowPositions();
}
void PlayerUi::openWindow(PanelWindow* wnd)
{
    wnd->setParent(window());
	m_windows.push_back(wnd);

	updateWindowPositions();
}

void PlayerUi::updateWindowPositions()
{
	int currentPosition = 0;
	for (PanelWindow* window : m_windows)
	{
		window->setWindowPosition(ls::Vec2I(0, currentPosition));
		currentPosition += window->absoluteWindowRect().height();
	}
}

void PlayerUi::onWindowUpdated(PanelWindow& window)
{
	updateWindowPositions();
}