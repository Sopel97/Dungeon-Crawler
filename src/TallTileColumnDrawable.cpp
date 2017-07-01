#include "TallTileColumnDrawable.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "tiles/renderers/TileRenderer.h"
#include "tiles/models/TileModel.h"

#include "TileLocation.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "GameConstants.h"

using namespace ls;

TallTileColumnDrawable::TallTileColumnDrawable(TileColumn& tileColumn, const TileLocation& tileLocation) :
    m_tileColumn(tileColumn),
    m_tileX(tileLocation.x),
    m_tileY(tileLocation.y),
    m_map(*tileLocation.map)
{
    int i = 0;
    for (TileStack& tileStack : m_tileColumn.tiles())
    {
        if (tileStack.tile().renderer().isTall())
        {
            m_indexOfFirstTallTile = i;
            m_boundingRectangle = tileStack.tile().model().collider({ m_tileX, m_tileY }).volume();
            m_center = m_boundingRectangle.centerOfMass();

            break;
        }
        ++i;
    }
}
TallTileColumnDrawable::~TallTileColumnDrawable()
{

}

const Rectangle2F& TallTileColumnDrawable::boundingRectangle() const
{
    return m_boundingRectangle;
}
const Vec2F& TallTileColumnDrawable::center() const
{
    return m_center;
}

TallDrawable::DrawableType TallTileColumnDrawable::type() const
{
    return TallDrawable::DrawableType::TileStack;
}

int TallTileColumnDrawable::tileX() const
{
    return m_tileX;
}
int TallTileColumnDrawable::tileY() const
{
    return m_tileY;
}

void TallTileColumnDrawable::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int tileStackSize = m_tileColumn.size();
    for (int i = m_indexOfFirstTallTile; i < tileStackSize; ++i)
    {
        m_tileColumn.at(i).tile().draw(renderTarget, renderStates, TileLocation(m_map, m_tileX, m_tileY, i));
    }
}
void TallTileColumnDrawable::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    int tileStackSize = m_tileColumn.size();
    for (int i = m_indexOfFirstTallTile; i < tileStackSize; ++i)
    {
        m_tileColumn.at(i).tile().drawMeta(renderTarget, renderStates, TileLocation(m_map, m_tileX, m_tileY, i));
    }
}
