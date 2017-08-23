#include "MapLayer.h"

#include "TileColumn.h"
#include "World.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "events/TilePlacedInWorld.h"
#include "events/TileRemovedFromWorld.h"

#include "TileLocation.h"

#include "EventDispatcher.h"

#include "../LibS/Util.h"

using namespace ls;

TileColumn MapLayer::m_emptyTileColumn {};

MapLayer::MapLayer(World& world, int width, int height) :
    m_world(world),
    m_width(width),
    m_height(height),
    m_tileColumns(width, height),
    m_lightCache(*this)
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
void MapLayer::placeTileMerge(TileStack&& tileStack, int x, int y)
{
    TileColumn& column = at(x, y);
    TileStack& top = column.top();
    if (top.tile().equals(tileStack.tile()))
    {
        const int spaceLeft = top.maxQuantity() - top.quantity();
        const int toMove = std::min(spaceLeft, tileStack.quantity());
        if (toMove > 0)
        {
            top.addTiles(toMove);
            tileStack.removeTiles(toMove);
        }
    }

    if (!tileStack.isEmpty())
    {
        placeTile(std::move(tileStack), x, y);
    }
}
TileStack MapLayer::takeTile(int x, int y)
{
    TileColumn& column = at(x, y);

    onTileRemoved(column.top(), x, y, column.topZ());

    return column.takeFromTop();
}

void MapLayer::addTiles(int x, int y, int z, int count)
{
    at(x, y, z).addTiles(count);
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
        stack.removeTiles(count);
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

std::vector<TileCollider> MapLayer::queryTileColliders(const Rectangle2I& queryRegion)
{
    std::vector<TileCollider> colliders;
    for(int x = queryRegion.min.x; x <= queryRegion.max.x; ++x)
    {
        for(int y = queryRegion.min.y; y <= queryRegion.max.y; ++y)
        {
            TileColumn& tileColumn = at(x, y);
            if (std::optional<TileCollider> collider = tileColumn.collider({ x,y }))
                colliders.emplace_back(collider.value());
        }
    }
    return colliders;
}
std::vector<ls::Rectangle2F> MapLayer::queryLightOccluders(const ls::Rectangle2I& queryRegion) const
{
    std::vector<ls::Rectangle2F> occluders;
    for (int x = queryRegion.min.x; x <= queryRegion.max.x; ++x)
    {
        for (int y = queryRegion.min.y; y <= queryRegion.max.y; ++y)
        {
            const TileColumn& tileColumn = at(x, y);
            if (std::optional<ls::Rectangle2F> occluder = tileColumn.lightOccluder({ x,y }))
                occluders.emplace_back(occluder.value());
        }
    }
    return occluders;
}
std::vector<Light> MapLayer::queryLights(const ls::Rectangle2I& queryRegion) const
{
    std::vector<Light> lights;

    for (const auto& pos : m_lightCache.queryLightPositions(queryRegion))
    {
        if (std::optional<Light> light = m_tileColumns(pos.x, pos.y).light(pos))
        {
            lights.emplace_back(light.value());
        }
    }

    return lights;
}

void MapLayer::onTilePlaced(TileStack& stack, int x, int y, int z)
{
    TileLocation location(*this, x, y, z);

    stack.tile().onTilePlaced(location);
    updateNearbyTiles(x, y, z, &Tile::onTilePlacedNearby);
    if (z > 0) m_tileColumns(x, y).at(z - 1).tile().onTilePlacedOnTop(TileLocation(*this, x, y, z - 1));
    EventDispatcher::instance().broadcast<TilePlacedInWorld>(TilePlacedInWorld{ &stack, location });

    m_lightCache.update(ls::Vec2I(x, y));
}
void MapLayer::onTileRemoved(TileStack& stack, int x, int y, int z)
{
    TileLocation location(*this, x, y, z);

    stack.tile().onTileRemoved(location);
    updateNearbyTiles(x, y, z, &Tile::onTileRemovedNearby);
    if (z > 0) m_tileColumns(x, y).at(z - 1).tile().onTileRemovedFromTop(TileLocation(*this, x, y, z - 1));
    EventDispatcher::instance().broadcast<TileRemovedFromWorld>(TileRemovedFromWorld{ &stack, location });

    m_lightCache.update(ls::Vec2I(x, y));
}

void MapLayer::updateNearbyTiles(int x, int y, int z, TileUpdateFunction func)
{
    const int minX = std::max(x - 1, 0);
    const int maxX = std::min(x + 1, m_width - 1);
    const int minY = std::max(y - 1, 0);
    const int maxY = std::min(y + 1, m_height - 1);

    for (int xx = minX; xx <= maxX; ++xx)
    {
        for (int yy = minY; yy <= maxY; ++yy)
        {
            if (xx == x && yy == y) continue;

            TileColumn& tileColumn = m_tileColumns(xx, yy);
            if (tileColumn.size() > z)
            {
                (tileColumn.at(z).tile().*func)(TileLocation(*this, xx, yy, z), ls::Vec2I(x - xx, y - yy));
            }
        }
    }
}