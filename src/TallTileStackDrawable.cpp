#include "TallTileStackDrawable.h"

#include "Tile.h"
#include "TileView.h"
#include "TileModel.h"

#include "TileStack.h"
#include "MapLayer.h"

using namespace Geo;

TallTileStackDrawable::TallTileStackDrawable(const TileStack& tileStack, int x, int y, const MapLayer& map) :
    m_tileStack(tileStack),
    m_tileX(x),
    m_tileY(y),
    m_map(map)
{
    int i = 0;
    for(const auto& tile : m_tileStack.tiles())
    {
        if(tile->view().isTall())
        {
            m_indexOfFirstTallTile = i;
            m_boundingRectangle = tile->model().collider();
            m_center = m_boundingRectangle.center();

            break;
        }
        ++i;
    }
}
TallTileStackDrawable::~TallTileStackDrawable()
{

}

bool TallTileStackDrawable::isTile() const
{
    return true;
}
const RectangleF& TallTileStackDrawable::boundingRectangle() const
{
    return m_boundingRectangle;
}
const Vec2F& TallTileStackDrawable::center() const
{
    return m_center;
}

int TallTileStackDrawable::tileX() const
{
    return m_tileX;
}
int TallTileStackDrawable::tileY() const
{
    return m_tileY;
}

void TallTileStackDrawable::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int tileStackSize = m_tileStack.size();
    for(int i = m_indexOfFirstTallTile; i < tileStackSize; ++i)
    {
        m_tileStack.at(i).draw(renderTarget, renderStates, m_tileX, m_tileY, i, m_map);
    }
}
