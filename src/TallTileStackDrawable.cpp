#include "TallTileStackDrawable.h"

#include "Tile.h"
#include "TileView.h"
#include "TileModel.h"

#include "TileLocation.h"

#include "TileStack.h"
#include "MapLayer.h"

using namespace Geo;

TallTileStackDrawable::TallTileStackDrawable(const TileStack& tileStack, const TileLocation& tileLocation) :
    m_tileStack(tileStack),
    m_tileX(tileLocation.x),
    m_tileY(tileLocation.y),
    m_map(tileLocation.map)
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

const RectangleF& TallTileStackDrawable::boundingRectangle() const
{
    return m_boundingRectangle;
}
const Vec2F& TallTileStackDrawable::center() const
{
    return m_center;
}

TallDrawable::DrawableType TallTileStackDrawable::type() const
{
    return TallDrawable::DrawableType::TileStack;
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
        m_tileStack.at(i).draw(renderTarget, renderStates, TileLocation(m_map, m_tileX, m_tileY, i));
    }
}
