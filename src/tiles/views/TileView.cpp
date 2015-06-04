#include "TileView.h"

#include "../LibS/make_unique.h"

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

void TileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
{

}

void TileView::loadFromConfiguration(ConfigurationNode& config)
{

}

const Tile* TileView::owner() const
{
    return m_owner;
}

void TileView::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}

int TileView::innerBorderGroup() const
{
    return -1;
}

std::unique_ptr<TileView> TileView::clone() const
{
    return std::make_unique<TileView>(*this);
}
