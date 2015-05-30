#include "TileView.h"


TileView::TileView(Tile* owner) :
    m_owner(owner)
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
