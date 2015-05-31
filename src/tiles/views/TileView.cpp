#include "TileView.h"


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
