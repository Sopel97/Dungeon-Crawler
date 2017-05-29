#include "MapLayer.h"

#include "TileColumn.h"
#include "World.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "TileLocation.h"

#include "../LibS/Util.h"

#include "GameConstants.h"

using namespace ls;

TileColumn MapLayer::m_emptyTileColumn {};

MapLayer::MapLayer(World& world, int width, int height) :
    m_world(world),
    m_width(width),
    m_height(height),
    m_tileColumns(width, height)
{

}

const Array2<TileColumn>& MapLayer::tileColumns() const
{
    return m_tileColumns;
}
Array2<TileColumn>& MapLayer::tileColumns()
{
    return m_tileColumns;
}

int MapLayer::width() const
{
    return m_width;
}
int MapLayer::height() const
{
    return m_height;
}

bool MapLayer::isValid(int x, int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

const TileColumn& MapLayer::at(int x, int y) const
{
    if(!isValid(x, y)) return m_emptyTileColumn;
    return m_tileColumns(x, y);
}
TileColumn& MapLayer::at(int x, int y)
{
    if(!isValid(x, y)) return m_emptyTileColumn;
    return m_tileColumns(x, y);
}
const TileStack& MapLayer::at(int x, int y, int z) const
{
    return at(x, y).at(z);
}
TileStack& MapLayer::at(int x, int y, int z)
{
    return at(x, y).at(z);
}

void MapLayer::placeTile(TileStack&& tileStack, int x, int y)
{
    at(x, y).placeOnTop(std::move(tileStack));
}
TileStack MapLayer::takeTile(int x, int y)
{
    return at(x, y).takeFromTop();
}

std::vector<Rectangle2F> MapLayer::queryTileColliders(const Rectangle2F& queryRegion) const
{
    const Vec2F& queryRegionTopLeft     = queryRegion.min;
    const Vec2F& queryRegionBottomRight = queryRegion.max;
    int firstTileX = std::max(Util::fastFloor(queryRegionTopLeft.x / GameConstants::tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(queryRegionTopLeft.y / GameConstants::tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(queryRegionBottomRight.x / GameConstants::tileSize), m_width - 1);
    int lastTileY = std::min(Util::fastFloor(queryRegionBottomRight.y / GameConstants::tileSize), m_height - 1);

    std::vector<Rectangle2F> colliders;
    for(int x = firstTileX; x <= lastTileX; ++x)
    {
        for(int y = firstTileY; y <= lastTileY; ++y)
        {
            const TileColumn& tileColumn = at(x, y);
            for(const TileStack& tileStack : tileColumn.tiles())
            {
                if(tileStack.tile().model().hasCollider())
                {
                    colliders.emplace_back(tileStack.tile().model().collider());
                }
            }
        }
    }
    return colliders;
}
