#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

int InventoryView::m_minSensibleHeight = 67;
int InventoryView::m_topBarHeight = 15;
int InventoryView::m_leftBarWidth = 4;
int InventoryView::m_bottomBarHeight = 4;
int InventoryView::m_rightBarWidth = 4;

InventoryView::InventoryView(Inventory* parent) :
    m_parentInventory(parent),
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(m_minSensibleHeight),
    m_minHeight(m_minSensibleHeight),
    m_maxHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(Inventory* parent, const std::vector<InventorySlotView>& slots) :
    m_parentInventory(parent),
    m_slotViews(slots),
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(m_minSensibleHeight),
    m_minHeight(m_minSensibleHeight),
    m_maxHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{
    for(const auto& slot : m_slotViews)
    {
        int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        m_maxHeight = std::max(m_maxHeight, slotBottom + 4 + m_bottomBarHeight + m_topBarHeight);
    }
}
InventoryView::InventoryView(Inventory* parent, std::vector<InventorySlotView>&& slots) :
    m_parentInventory(parent),
    m_slotViews(std::move(slots)),
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(m_minSensibleHeight),
    m_minHeight(m_minSensibleHeight),
    m_maxHeight(m_minSensibleHeight),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{
    for(const auto& slot : m_slotViews)
    {
        int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        m_maxHeight = std::max(m_maxHeight, slotBottom + 4 + m_bottomBarHeight + m_topBarHeight);
    }
}

void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
    int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
    m_maxHeight = std::max(m_maxHeight, slotBottom + 4 + m_bottomBarHeight + m_topBarHeight);
}

void InventoryView::setOffsetFromTop(int newOffset)
{
    m_offsetFromTop = newOffset;
}

Inventory const* InventoryView::parentInventory() const
{
    return m_parentInventory;
}
int InventoryView::height() const
{
    return m_height;
}
int InventoryView::minHeight() const
{
    return m_minHeight;
}
int InventoryView::maxHeight() const
{
    return m_maxHeight;
}

bool InventoryView::isMinimizable() const
{
    return m_isMinimizable;
}
bool InventoryView::isCloseable() const
{
    return m_isCloseable;
}
bool InventoryView::isResizeable() const
{
    return m_isResizeable;
}

void InventoryView::setMinimizable(bool newValue)
{
    m_isMinimizable = newValue;
}
void InventoryView::setCloseable(bool newValue)
{
    m_isCloseable = newValue;
}
void InventoryView::setResizeable(bool newValue)
{
    m_isResizeable = newValue;
}

void InventoryView::setInnerHeight(int newInnerHeight)
{
    m_height = newInnerHeight + m_topBarHeight + m_bottomBarHeight;
}

void InventoryView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    constexpr int inventoryWindowWidth = 240;
    const Vec2I topBarSpritePosition {0, 0};
    const Vec2I leftBarSpritePosition {0, 0};
    const Vec2I rightBarSpritePosition {4, 0};
    const Vec2I bottomBarSpritePosition {0, 15};
    const Vec2I scrollBarSpritePosition {8, 0};
    const Vec2I scrollBarUpButtonSpritePosition {0, 59};
    const Vec2I scrollBarDownButtonSpritePosition {12, 59};
    const Vec2I scrollBarSliderButtonSpritePosition {24, 59};
    const Vec2I scrollButtonsSizes {12, 12};

    ResourceHandle<sf::Texture> backgroundTexture = ResourceManager::instance().get<sf::Texture>("UiBackground");
    ResourceHandle<sf::Texture> verticalBarsTexture = ResourceManager::instance().get<sf::Texture>("UiVerticalBars");
    ResourceHandle<sf::Texture> otherTexture = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    RectangleI playerUiPanelRect = Root::instance().windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::PlayerUi);
    RectangleI inventoryWindowRect(playerUiPanelRect.min + Vec2I(0, m_offsetFromTop), playerUiPanelRect.width(), m_height);
    RectangleI inventoryContentRect(inventoryWindowRect.min + Vec2I(m_leftBarWidth, m_topBarHeight), inventoryWindowRect.max - Vec2I(m_rightBarWidth, m_bottomBarHeight));
    int sideBarHeight = inventoryContentRect.height();

    Root::instance().windowSpaceManager().setDefaultView();

    sf::Sprite topPanelSprite;
    topPanelSprite.setPosition(inventoryWindowRect.min.x, inventoryWindowRect.min.y);
    topPanelSprite.setTexture(otherTexture.get());
    topPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(topBarSpritePosition.x, topBarSpritePosition.y), sf::Vector2i(inventoryWindowWidth, m_topBarHeight)));
    renderTarget.draw(topPanelSprite, renderStates);

    sf::Sprite leftBarSprite;
    leftBarSprite.setPosition(inventoryWindowRect.min.x, inventoryWindowRect.min.y + m_topBarHeight);
    leftBarSprite.setTexture(verticalBarsTexture.get());
    leftBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(leftBarSpritePosition.x, leftBarSpritePosition.y), sf::Vector2i(m_leftBarWidth, sideBarHeight)));
    renderTarget.draw(leftBarSprite, renderStates);

    sf::Sprite rightBarSprite;
    rightBarSprite.setPosition(inventoryWindowRect.max.x - m_rightBarWidth, inventoryWindowRect.min.y + m_topBarHeight);
    rightBarSprite.setTexture(verticalBarsTexture.get());
    rightBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(rightBarSpritePosition.x, rightBarSpritePosition.y), sf::Vector2i(m_rightBarWidth, sideBarHeight)));
    renderTarget.draw(rightBarSprite, renderStates);

    sf::Sprite bottomPanelSprite;
    bottomPanelSprite.setPosition(inventoryWindowRect.min.x, inventoryWindowRect.max.y - m_bottomBarHeight);
    bottomPanelSprite.setTexture(otherTexture.get());
    bottomPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(bottomBarSpritePosition.x, bottomBarSpritePosition.y), sf::Vector2i(inventoryWindowWidth, m_bottomBarHeight)));
    renderTarget.draw(bottomPanelSprite, renderStates);

    sf::Sprite backgroundSprite;
    backgroundSprite.setPosition(inventoryContentRect.min.x, inventoryContentRect.min.y);
    backgroundSprite.setTexture(backgroundTexture.get());
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, m_scroll), sf::Vector2i(inventoryWindowWidth - m_leftBarWidth - m_rightBarWidth, sideBarHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    //if(m_isResizeable)
    {
        constexpr int scrollBarWidth = 12;
        int scrollBarHeight = inventoryContentRect.height();

        const Vec2F scrollBarTopLeft {inventoryContentRect.max.x - scrollBarWidth, inventoryContentRect.min.y};

        sf::Sprite scrollBarSprite;
        scrollBarSprite.setPosition(scrollBarTopLeft.x, scrollBarTopLeft.y);
        scrollBarSprite.setTexture(verticalBarsTexture.get());
        scrollBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(scrollBarSpritePosition.x, scrollBarSpritePosition.y), sf::Vector2i(scrollBarWidth, scrollBarHeight)));
        renderTarget.draw(scrollBarSprite, renderStates);

        sf::Sprite scrollBarUpButtonSprite;
        scrollBarUpButtonSprite.setPosition(scrollBarTopLeft.x, scrollBarTopLeft.y);
        scrollBarUpButtonSprite.setTexture(otherTexture.get());
        scrollBarUpButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(scrollBarUpButtonSpritePosition.x, scrollBarUpButtonSpritePosition.y), sf::Vector2i(scrollButtonsSizes.x, scrollButtonsSizes.y)));
        renderTarget.draw(scrollBarUpButtonSprite, renderStates);

        sf::Sprite scrollBarDownButtonSprite;
        scrollBarDownButtonSprite.setPosition(scrollBarTopLeft.x, scrollBarTopLeft.y + scrollBarHeight - scrollButtonsSizes.y);
        scrollBarDownButtonSprite.setTexture(otherTexture.get());
        scrollBarDownButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(scrollBarDownButtonSpritePosition.x, scrollBarDownButtonSpritePosition.y), sf::Vector2i(scrollButtonsSizes.x, scrollButtonsSizes.y)));
        renderTarget.draw(scrollBarDownButtonSprite, renderStates);

        if(m_height != m_maxHeight)
        {
            int sliderMin = scrollBarTopLeft.y + scrollButtonsSizes.y / 2 + scrollButtonsSizes.y;
            int sliderMax = scrollBarTopLeft.y + scrollBarHeight - scrollButtonsSizes.y / 2 - 2*scrollButtonsSizes.y;
            int sliderRangeLength = sliderMax - sliderMin;
            int contentViewHeight = m_height - m_topBarHeight - m_bottomBarHeight;
            int fullContentHeight = m_maxHeight - m_topBarHeight - m_bottomBarHeight;
            int scrollRangeLength = fullContentHeight - contentViewHeight;
            float scrolled = static_cast<float>(m_scroll) / static_cast<float>(scrollRangeLength);
            int sliderPosition = sliderMin + sliderRangeLength * scrolled;

            sf::Sprite scrollBarSliderButtonSprite;
            scrollBarSliderButtonSprite.setPosition(scrollBarTopLeft.x, sliderPosition - scrollButtonsSizes.y / 2);
            scrollBarSliderButtonSprite.setTexture(otherTexture.get());
            scrollBarSliderButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(scrollBarSliderButtonSpritePosition.x, scrollBarSliderButtonSpritePosition.y), sf::Vector2i(scrollButtonsSizes.x, scrollButtonsSizes.y)));
            renderTarget.draw(scrollBarSliderButtonSprite, renderStates);
        }
    }

    Root::instance().windowSpaceManager().setViewToRect(inventoryContentRect, RectangleF(Vec2F(0, m_scroll), inventoryWindowWidth - m_leftBarWidth - m_rightBarWidth, sideBarHeight));
    for(auto& slot : m_slotViews) slot.draw(renderTarget, renderStates);

}
void InventoryView::update()
{
    int contentViewHeight = m_height - m_topBarHeight - m_bottomBarHeight;
    int fullContentHeight = m_maxHeight - m_topBarHeight - m_bottomBarHeight;

    m_height = std::min(std::max(m_height, m_minHeight), m_maxHeight);
    m_scroll = std::min(std::max(m_scroll, 0), fullContentHeight - contentViewHeight);
}
