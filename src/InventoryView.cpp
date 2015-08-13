#include "InventoryView.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

int InventoryView::m_topBarSize = 15;
int InventoryView::m_leftMarginSize = 4;
int InventoryView::m_bottomMarginSize = 4;
int InventoryView::m_rightMarginSize = 4;

InventoryView::InventoryView() :
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(67),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(const std::vector<InventorySlotView>& slots) :
    m_slotViews(slots),
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(67),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}
InventoryView::InventoryView(std::vector<InventorySlotView>&& slots) :
    m_slotViews(std::move(slots)),
    m_offsetFromTop(0),
    m_scroll(0),
    m_height(67),
    m_isMinimizable(true),
    m_isCloseable(true),
    m_isResizeable(true)
{

}

void InventoryView::addInventorySlotView(const InventorySlotView& slot)
{
    m_slotViews.push_back(slot);
}

void InventoryView::setOffsetFromTop(int newOffset)
{
    m_offsetFromTop = newOffset;
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
    m_height = newInnerHeight + m_topBarSize + m_bottomMarginSize;
}

void InventoryView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    ResourceHandle<sf::Texture> backgroundTexture = ResourceManager::instance().get<sf::Texture>("UiBackground");
    ResourceHandle<sf::Texture> verticalBarsTexture = ResourceManager::instance().get<sf::Texture>("UiVerticalBars");
    ResourceHandle<sf::Texture> otherTexture = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");

    RectangleI playerUiPanelRect = Root::instance().windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::PlayerUi);
    RectangleI inventoryRect(playerUiPanelRect.min + Vec2I(0, m_offsetFromTop), playerUiPanelRect.width(), m_height);
    RectangleI innerInventoryRect(inventoryRect.min + Vec2I(m_leftMarginSize, m_topBarSize), inventoryRect.max - Vec2I(m_rightMarginSize, m_bottomMarginSize));

    Root::instance().windowSpaceManager().setDefaultView();

    sf::Sprite topPanelSprite;
    topPanelSprite.setPosition(inventoryRect.min.x, inventoryRect.min.y);
    topPanelSprite.setTexture(otherTexture.get());
    topPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(240, 15)));
    renderTarget.draw(topPanelSprite, renderStates);

    int barHeight = m_height - m_topBarSize - m_bottomMarginSize;

    sf::Sprite leftBarSprite;
    leftBarSprite.setPosition(inventoryRect.min.x, inventoryRect.min.y + m_topBarSize);
    leftBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(4, barHeight)));
    leftBarSprite.setTexture(verticalBarsTexture.get());
    renderTarget.draw(leftBarSprite, renderStates);

    sf::Sprite rightBarSprite;
    rightBarSprite.setPosition(inventoryRect.max.x - m_rightMarginSize, inventoryRect.min.y + m_topBarSize);
    rightBarSprite.setTextureRect(sf::IntRect(sf::Vector2i(4, 0), sf::Vector2i(4, barHeight)));
    rightBarSprite.setTexture(verticalBarsTexture.get());
    renderTarget.draw(rightBarSprite, renderStates);

    sf::Sprite bottomPanelSprite;
    bottomPanelSprite.setPosition(inventoryRect.min.x, inventoryRect.max.y - m_bottomMarginSize);
    bottomPanelSprite.setTexture(otherTexture.get());
    bottomPanelSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 15), sf::Vector2i(240, 4)));
    renderTarget.draw(bottomPanelSprite, renderStates);

    sf::Sprite backgroundSprite;
    backgroundSprite.setPosition(inventoryRect.min.x + m_leftMarginSize, inventoryRect.min.y + m_topBarSize);
    backgroundSprite.setTexture(backgroundTexture.get());
    backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, m_scroll), sf::Vector2i(240 - m_leftMarginSize - m_rightMarginSize, barHeight)));
    renderTarget.draw(backgroundSprite, renderStates);

    Root::instance().windowSpaceManager().setViewToRect(innerInventoryRect, RectangleF(Vec2F(0, m_scroll), 240 - m_leftMarginSize - m_rightMarginSize, barHeight));

    for(auto& slot : m_slotViews) slot.draw(renderTarget, renderStates);
}
