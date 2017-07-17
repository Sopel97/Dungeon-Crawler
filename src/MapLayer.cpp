#include "MapLayer.h"

#include "TileColumn.h"
#include "World.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "events/TilePlacedInWorld.h"
#include "events/TileRemovedFromWorld.h"

#include "TileLocation.h"

#include "EventDispatcher.h"

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
    TileColumn& column = at(x, y);
    TileStack& stack = column.placeOnTop(std::move(tileStack));

    onTilePlaced(stack, x, y, column.topZ());
}
TileStack MapLayer::takeTile(int x, int y)
{
    TileColumn& column = at(x, y);

    onTileRemoved(column.top(), x, y, column.topZ());

    return column.takeFromTop();
}

void MapLayer::addTiles(int x, int y, int z, int count)
{
    at(x, y, z).insert(count);
}
void MapLayer::removeTiles(int x, int y, int z, int count)
{
    TileColumn& column = at(x, y);
    TileStack& stack = column.at(z);

    if (stack.quantity() <= count)
    {
        onTileRemoved(stack, x, y, z);
        column.take(z);
    }
    else
    {
        stack.erase(count);
    }
}
TileStack MapLayer::splitTiles(int x, int y, int z, int count)
{
    TileColumn& column = at(x, y);
    TileStack& stack = column.at(z);

    if (stack.quantity() <= count)
    {
        onTileRemoved(stack, x, y, z);
        return column.take(z);
    }
    else
    {
        return stack.split(count);
    }
}

std::vector<TileCollider> MapLayer::queryTileColliders(const Rectangle2F& queryRegion)
{
    const Vec2F& queryRegionTopLeft     = queryRegion.min;
    const Vec2F& queryRegionBottomRight = queryRegion.max;
    int firstTileX = std::max(Util::fastFloor(queryRegionTopLeft.x / GameConstants::tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(queryRegionTopLeft.y / GameConstants::tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(queryRegionBottomRight.x / GameConstants::tileSize), m_width - 1);
    int lastTileY = std::min(Util::fastFloor(queryRegionBottomRight.y / GameConstants::tileSize), m_height - 1);

    std::vector<TileCollider> colliders;
    for(int x = firstTileX; x <= lastTileX; ++x)
    {
        for(int y = firstTileY; y <= lastTileY; ++y)
        {
            TileColumn& tileColumn = at(x, y);
            if (tileColumn.hasCollider()) colliders.emplace_back(tileColumn.collider({ x, y }));
        }
    }
    return colliders;
}

void MapLayer::onTilePlaced(TileStack& stack, int x, int y, int z)
{
    TileLocation location(*this, x, y, z);

    stack.tile().onTilePlaced(location);
    EventDispatcher::instance().broadcast<TilePlacedInWorld>(TilePlacedInWorld{ &stack, location });
}
void MapLayer::onTileRemoved(TileStack& stack, int x, int y, int z)
{
    TileLocation location(*this, x, y, z);

    stack.tile().onTileRemoved(location);
    EventDispatcher::instance().broadcast<TileRemovedFromWorld>(TileRemovedFromWorld{ &stack, location });
}