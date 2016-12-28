#include "tiles/views/TileView.h"

#include "SFML/Graphics.hpp"
#include "TileLocation.h"
#include "GameConstants.h"

TileView::TileView() :
    m_owner(nullptr)
{

}
TileView::TileView(const TileView& other) :
    m_owner(other.m_owner)
{

}
TileView::~TileView()
{

}

void TileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{

}
void TileView::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{

}
void TileView::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    sf::RectangleShape spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x) * GameConstants::tileSize, static_cast<float>(location.y) * GameConstants::tileSize));
    spr.setFillColor(sf::Color::Black);
    renderTarget.draw(spr, renderStates);
}

void TileView::loadFromConfiguration(ConfigurationNode& config)
{

}

void TileView::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}

int TileView::innerBorderGroup() const
{
    return -1;
}

int TileView::outerBorderPriority() const
{
    return -1;
}
bool TileView::hasOuterBorder() const
{
    return false;
}
bool TileView::coversOuterBorders() const
{
    return true;
}

bool TileView::isTall() const
{
    return false;
}

void TileView::onTilePlaced(const TileLocation& location)
{

}
void TileView::onTileRemoved(const TileLocation& location)
{

}
void TileView::onTileQuantityChanged(int oldQuantity, int newQuantity)
{

}
void TileView::onTileInstantiated()
{

}

std::unique_ptr<ComponentCommonData> TileView::createCommonDataStorage() const
{
    return nullptr;
}
void TileView::setCommonDataStorage(ComponentCommonData& commonData)
{

}

std::unique_ptr<TileView> TileView::clone() const
{
    return std::make_unique<TileView>(*this);
}
