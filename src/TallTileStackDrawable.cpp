#include "TallTileStackDrawable.h"

#include "tiles/Tile.h"
#include "tiles/views/TileView.h"
#include "tiles/models/TileModel.h"

#include "TileLocation.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "GameConstants.h"

using namespace ls;

TallTileStackDrawable::TallTileStackDrawable(const TileColumn& tileColumn, const TileLocation& tileLocation) :
    m_tileColumn(tileColumn),
    m_tileX(tileLocation.x),
    m_tileY(tileLocation.y),
    m_map(tileLocation.map)
{
    int i = 0;
    for(const auto& tile : m_tileColumn.tiles())
    {
        if(tile->view().isTall())
        {
            m_indexOfFirstTallTile = i;
            m_boundingRectangle = tile->model().collider() + Vec2F(static_cast<float>(tileLocation.x), static_cast<float>(tileLocation.y)) * static_cast<float>(GameConstants::tileSize);
            m_center = m_boundingRectangle.centerOfMass();

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
    int tileStackSize = m_tileColumn.size();
    for(int i = m_indexOfFirstTallTile; i < tileStackSize; ++i)
    {
        m_tileColumn.at(i).draw(renderTarget, renderStates, TileLocation(m_map, m_tileX, m_tileY, i));
    }
}
