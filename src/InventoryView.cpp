#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/Geometry.h"

using namespace ls;

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
    updateMaxHeight();
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
    updateMaxHeight();
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
int InventoryView::offsetFromTop() const
{
    return m_offsetFromTop;
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

void InventoryView::setHeight(int newHeight)
{
    m_height = newHeight;
}
void InventoryView::setInnerHeight(int newInnerHeight)
{
    m_height = newInnerHeight + m_topBarHeight + m_bottomBarHeight;
}
void InventoryView::setHeightToMax()
{
    m_height = m_maxHeight;
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

    const Vec2F& inventoryContentTopLeft(inventoryContentRect.min);
    const Vec2F& inventoryWindowTopLeft(inventoryWindowRect.min);
    const Vec2F& inventoryWindowBottomRight(inventoryWindowRect.max); 

    sf::Sprite topPanelSprite;
    topPanelSprite.setPosition(inventoryWindowTopLeft.x, inventoryWindowTopLeft.y);
    topPanelSprite.setTexture(otherTexture.get());
    topPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(topBarSpritePosition.x, topBarSpritePosition.y), sf::Vector2i(inventoryWindowWidth, m_topBarHeight)));
    renderTarget.draw(topPanelSprite, renderStates);

    sf::Sprite leftBarSprite;
    leftBarSprite.setPosition(inventoryWindowTopLeft.x, inventoryWindowTopLeft.y + m_topBarHeight);
    leftBarSprite.setTexture(verticalBarsTexture.get());
    leftBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(leftBarSpritePosition.x, leftBarSpritePosition.y), sf::Vector2i(m_leftBarWidth, sideBarHeight)));
    renderTarget.draw(leftBarSprite, renderStates);

    sf::Sprite rightBarSprite;
    rightBarSprite.setPosition(inventoryWindowBottomRight.x - m_rightBarWidth, inventoryWindowTopLeft.y + m_topBarHeight);
    rightBarSprite.setTexture(verticalBarsTexture.get());
    rightBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(rightBarSpritePosition.x, rightBarSpritePosition.y), sf::Vector2i(m_rightBarWidth, sideBarHeight)));
    renderTarget.draw(rightBarSprite, renderStates);

    sf::Sprite bottomPanelSprite;
    bottomPanelSprite.setPosition(inventoryWindowTopLeft.x, inventoryWindowBottomRight.y - m_bottomBarHeight);
    bottomPanelSprite.setTexture(otherTexture.get());
    bottomPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(bottomBarSpritePosition.x, bottomBarSpritePosition.y), sf::Vector2i(inventoryWindowWidth, m_bottomBarHeight)));
    renderTarget.draw(bottomPanelSprite, renderStates);

    sf::Sprite backgroundSprite;
    backgroundSprite.setPosition(inventoryContentTopLeft.x, inventoryContentTopLeft.y);
    backgroundSprite.setTexture(backgroundTexture.get());
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, m_scroll), sf::Vector2i(inventoryWindowWidth - m_leftBarWidth - m_rightBarWidth, sideBarHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    if(m_isResizeable)
    {
        constexpr int scrollBarWidth = 12;
        int scrollBarHeight = inventoryContentRect.height();

        const Vec2F scrollBarTopLeft(static_cast<float>(inventoryContentRect.max.x - scrollBarWidth), static_cast<float>(inventoryContentRect.min.y));

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
            int sliderMin = static_cast<int>(scrollBarTopLeft.y) + scrollButtonsSizes.y / 2 + scrollButtonsSizes.y;
            int sliderMax = static_cast<int>(scrollBarTopLeft.y) + scrollBarHeight - scrollButtonsSizes.y / 2 - 2 * scrollButtonsSizes.y;
            int sliderRangeLength = sliderMax - sliderMin;
            int contentViewHeight = m_height - m_topBarHeight - m_bottomBarHeight;
            int fullContentHeight = m_maxHeight - m_topBarHeight - m_bottomBarHeight;
            int scrollRangeLength = fullContentHeight - contentViewHeight;
            float scrolled = static_cast<float>(m_scroll) / static_cast<float>(scrollRangeLength);
            int sliderPosition = static_cast<int>(sliderMin + sliderRangeLength * scrolled);

            sf::Sprite scrollBarSliderButtonSprite;
            scrollBarSliderButtonSprite.setPosition(scrollBarTopLeft.x, static_cast<float>(sliderPosition - scrollButtonsSizes.y / 2));
            scrollBarSliderButtonSprite.setTexture(otherTexture.get());
            scrollBarSliderButtonSprite.setTextureRect(sf::IntRect(sf::Vector2i(scrollBarSliderButtonSpritePosition.x, scrollBarSliderButtonSpritePosition.y), sf::Vector2i(scrollButtonsSizes.x, scrollButtonsSizes.y)));
            renderTarget.draw(scrollBarSliderButtonSprite, renderStates);
        }
    }

    Root::instance().windowSpaceManager().setViewToRect(inventoryContentRect, RectangleF(Vec2F(0.0f, static_cast<float>(m_scroll)), static_cast<float>(inventoryWindowWidth - m_leftBarWidth - m_rightBarWidth), static_cast<float>(sideBarHeight)));
    for(auto& slot : m_slotViews) slot.draw(renderTarget, renderStates);

}
void InventoryView::update()
{
    int contentViewHeight = m_height - m_topBarHeight - m_bottomBarHeight;
    int fullContentHeight = m_maxHeight - m_topBarHeight - m_bottomBarHeight;

    m_height = std::min(std::max(m_height, m_minHeight), m_maxHeight);
    m_scroll = std::min(std::max(m_scroll, 0), fullContentHeight - contentViewHeight);
}

void InventoryView::updateMaxHeight()
{
    for(const auto& slot : m_slotViews)
    {
        int slotBottom = slot.position().y + InventorySlotView::slotSize().y;
        m_maxHeight = std::max(m_maxHeight, slotBottom + 4 + m_bottomBarHeight + m_topBarHeight);
    }
}
