#include "MapLayer.h"

#include "TileColumn.h"
#include "World.h"
#include "tiles/models/TileModel.h"
#include "tiles/views/TileView.h"
#include "tiles/controllers/TileController.h"

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

MapLayer::~MapLayer()
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
const Tile& MapLayer::at(int x, int y, int z) const
{
    return at(x, y).at(z);
}
Tile& MapLayer::at(int x, int y, int z)
{
    return at(x, y).at(z);
}

void MapLayer::placeTile(Tile* tile, int x, int y)
{
    TileColumn& tileColumn = at(x, y);
    tileColumn.push(tile);
    tile->onTilePlaced(TileLocation(*this, x, y, tileColumn.topZ()));
}
Tile* MapLayer::takeTile(int x, int y)
{
    TileColumn& tileColumn = at(x, y);
    int z = tileColumn.topZ();
    Tile* tile = tileColumn.releaseTop();
    tile->onTileRemoved(TileLocation(*this, x, y, z));
    return tile;
}
void MapLayer::deleteTile(int x, int y)
{
    delete takeTile(x, y);
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
            for(const auto& tile : tileColumn.tiles())
            {
                if(tile->model().hasCollider())
                {
                    colliders.emplace_back(tile->model().collider());
                }
            }
        }
    }
    return colliders;
}
