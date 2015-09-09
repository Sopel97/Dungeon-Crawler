#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Game.h"
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
const Vec2I PlayerUi::m_windowHighTopLeftCornerSize = {m_windowLeftBarWidth, m_windowHeaderHeight};
const Vec2I PlayerUi::m_windowLowTopLeftCornerSize = {m_windowLeftBarWidth, m_windowTopBarHeight};
const Vec2I PlayerUi::m_windowHighTopRightCornerSize = {m_windowRightBarWidth, m_windowTopBarHeight};
const Vec2I PlayerUi::m_windowLowTopRightCornerSize = {m_windowRightBarWidth, m_windowTopBarHeight};
const Vec2I PlayerUi::m_windowBottomLeftCornerSize = {m_windowLeftBarWidth, m_windowBottomBarHeight};
const Vec2I PlayerUi::m_windowBottomRightCornerSize = {m_windowRightBarWidth, m_windowBottomBarHeight};
const Vec2I PlayerUi::m_windowButtonSize = {12, 12};
const Vec2I PlayerUi::m_windowScrollSliderSize = {12, 12};

const Vec2I PlayerUi::m_windowTopBarSpritePosition = {0, 0};
const Vec2I PlayerUi::m_windowHeaderSpritePosition = {0, 8};
const Vec2I PlayerUi::m_windowLeftBarSpritePosition = {0, 0};
const Vec2I PlayerUi::m_windowRightBarSpritePosition = {4, 0};
const Vec2I PlayerUi::m_windowBottomBarSpritePosition = {0, 4};
const Vec2I PlayerUi::m_windowHighTopLeftCornerSpritePosition = {206, 19};
const Vec2I PlayerUi::m_windowLowTopLeftCornerSpritePosition = {198, 19};
const Vec2I PlayerUi::m_windowHighTopRightCornerSpritePosition = {210, 19};
const Vec2I PlayerUi::m_windowLowTopRightCornerSpritePosition = {202, 19};
const Vec2I PlayerUi::m_windowBottomLeftCornerSpritePosition = {198, 23};
const Vec2I PlayerUi::m_windowBottomRightCornerSpritePosition = {202, 23};
const Vec2I PlayerUi::m_windowScrollBarSpritePosition = {8, 0};
const Vec2I PlayerUi::m_windowScrollBarUpButtonSpritePosition = {0, 57};
const Vec2I PlayerUi::m_windowScrollBarDownButtonSpritePosition = {12, 57};
const Vec2I PlayerUi::m_windowScrollBarSliderSpritePosition = {24, 57};

PlayerUi::PlayerUi(Game& game, Player& player) :
    m_game(game),
    m_player(player)
{

}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for(auto& inv : m_game.inventorySystem().openedInventories())
    {
        inv->inventoryView.draw(*this, renderTarget, renderStates);
    }
}

void PlayerUi::drawWindowOnPanel(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window)
{
    RectangleI playerUiPanelRect = Root::instance().windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::PlayerUi);
    const int& topBarHeight = window.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    RectangleI windowRect(playerUiPanelRect.min + Vec2I(0, window.offsetFromTop()), m_playerUiPanelWidth, window.contentHeight() + m_windowBottomBarHeight + topBarHeight);

    drawWindow(renderTarget, renderStates, window, windowRect);
}
void PlayerUi::drawPopupWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window)
{
    const int& topBarHeight = window.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    RectangleI windowRect(window.position(), window.contentWidth() + m_windowLeftBarWidth + m_windowRightBarWidth, window.contentHeight() + m_windowBottomBarHeight + topBarHeight);

    drawWindow(renderTarget, renderStates, window, windowRect);
}
void PlayerUi::drawWindow(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Window& window, const RectangleI& rect)
{
    ResourceHandle<sf::Texture> backgroundTexture = ResourceManager::instance().get<sf::Texture>("UiBackground");
    ResourceHandle<sf::Texture> verticalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiVerticalBars");
    ResourceHandle<sf::Texture> horizontalBarsSprites = ResourceManager::instance().get<sf::Texture>("UiHorizontalBars");
    ResourceHandle<sf::Texture> nonRepeatingSprites = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    Root::instance().windowSpaceManager().setDefaultView();

    const int& topBarHeight = window.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;
    const Vec2I& topBarSpritePosition = window.hasHeader() ? m_windowHeaderSpritePosition : m_windowTopBarSpritePosition;
    const Vec2I& topLeftCornerSpritePosition = window.hasHeader() ? m_windowHighTopLeftCornerSpritePosition : m_windowLowTopLeftCornerSpritePosition;
    const Vec2I& topRightCornerSpritePosition = window.hasHeader() ? m_windowHighTopRightCornerSpritePosition : m_windowLowTopRightCornerSpritePosition;

    RectangleI windowInteriorRect(rect.min + Vec2I(m_windowLeftBarWidth, topBarHeight), rect.max - Vec2I(m_windowRightBarWidth, m_windowBottomBarHeight));

    const Vec2I& windowTopLeft(rect.min);
    const Vec2I& windowBottomRight(rect.max);
    const Vec2I& windowInteriorTopLeft(windowInteriorRect.min);
    const Vec2I& windowInteriorBottomRight(windowInteriorRect.max);

    int windowWidth = rect.width();
    int windowHeight = rect.height();
    int windowInteriorWidth = windowInteriorRect.width();
    int windowInteriorHeight = windowInteriorRect.height();

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
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, window.scroll()), sf::Vector2i(windowInteriorWidth, windowInteriorHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    if(window.hasScrollBar())
    {
        int scrollBarHeight = windowInteriorHeight;

        const Vec2I scrollBarTopLeft(windowInteriorRect.max.x - m_windowScrollBarWidth, windowInteriorRect.min.y);

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

        if(windowInteriorHeight < window.contentHeight())
        {
            int sliderMin = static_cast<int>(scrollBarTopLeft.y) + m_windowButtonSize.y / 2 + m_windowButtonSize.y;
            int sliderMax = static_cast<int>(scrollBarTopLeft.y) + scrollBarHeight - m_windowButtonSize.y / 2 - 2 * m_windowButtonSize.y;
            int sliderRangeLength = sliderMax - sliderMin;
            int scrollRangeLength = window.contentHeight() - windowInteriorHeight;
            float scrolled = static_cast<float>(window.scroll()) / static_cast<float>(scrollRangeLength);
            int sliderPosition = static_cast<int>(sliderMin + sliderRangeLength * scrolled);

            sf::Sprite scrollBarSliderButtonSprite;
            scrollBarSliderButtonSprite.setPosition(static_cast<float>(scrollBarTopLeft.x), static_cast<float>(sliderPosition - m_windowButtonSize.y / 2));
            scrollBarSliderButtonSprite.setTexture(nonRepeatingSprites.get());
            scrollBarSliderButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_windowScrollBarSliderSpritePosition.x, m_windowScrollBarSliderSpritePosition.y), sf::Vector2i(m_windowButtonSize.x, m_windowButtonSize.y)));
            renderTarget.draw(scrollBarSliderButtonSprite, renderStates);
        }
    }
}

void PlayerUi::setContentViewOfPanelWindow(Window& window)
{
    const int& topBarHeight = window.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;

    RectangleI playerUiPanelRect = Root::instance().windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::PlayerUi);
    RectangleI rect(playerUiPanelRect.min + Vec2I(0, window.offsetFromTop()), m_playerUiPanelWidth, window.windowHeight());

    RectangleI windowInteriorRect(rect.min + Vec2I(m_windowLeftBarWidth, topBarHeight), rect.max - Vec2I(m_windowRightBarWidth, m_windowBottomBarHeight));

    Root::instance().windowSpaceManager().setViewToRect(windowInteriorRect, RectangleF(Vec2F(0.0f, static_cast<float>(window.scroll())), static_cast<float>(window.contentWidth()), static_cast<float>(window.contentHeight())));
}

void PlayerUi::setContentViewOfPopupWindow(Window& window)
{
    const int& topBarHeight = window.hasHeader() ? m_windowHeaderHeight : m_windowTopBarHeight;

    RectangleI rect(window.position(), window.windowWidth(), window.windowHeight());

    RectangleI windowInteriorRect(rect.min + Vec2I(m_windowLeftBarWidth, topBarHeight), rect.max - Vec2I(m_windowRightBarWidth, m_windowBottomBarHeight));

    Root::instance().windowSpaceManager().setViewToRect(windowInteriorRect, RectangleF(Vec2F(0.0f, static_cast<float>(window.scroll())), static_cast<float>(window.contentWidth()), static_cast<float>(window.contentHeight())));
}

void PlayerUi::closeWindow(Window* window)
{
    for(auto& w : m_ownedWindows)
    {
        if(w == window)
        {
            delete window;
            break;
        }
    }

    m_panelWindows.erase(std::remove(m_panelWindows.begin(), m_panelWindows.end(), window), m_panelWindows.end());
    m_popupWindows.erase(std::remove(m_popupWindows.begin(), m_popupWindows.end(), window), m_popupWindows.end());
    m_ownedWindows.erase(std::remove(m_ownedWindows.begin(), m_ownedWindows.end(), window), m_ownedWindows.end());
}
void PlayerUi::openPanelWindow(Window* window)
{
    m_panelWindows.push_back(window);
}
void PlayerUi::openPopupWindow(Window* window)
{
    m_popupWindows.push_back(window);
}