#include "tiles/views/TileView.h"

TileView::TileView(Tile* owner) :
    m_owner(owner)
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

std::unique_ptr<TileView> TileView::clone() const
{
    return std::make_unique<TileView>(*this);
}
std::unique_ptr<TileView> TileView::create(Tile* owner) const
{
    return std::make_unique<TileView>(owner);
}
