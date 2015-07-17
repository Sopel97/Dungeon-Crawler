#include "MapLayer.h"

#include "TileStack.h"
#include "World.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileController.h"

#include "../LibS/Util.h"

using namespace Geo;

TileStack MapLayer::m_emptyTileStack {};

MapLayer::MapLayer(World& world, int width, int height) :
    m_world(world),
    m_width(width),
    m_height(height),
    m_tileStacks(width, height)
{

}

MapLayer::~MapLayer()
{

}

const Array2<TileStack>& MapLayer::tileStacks() const
{
    return m_tileStacks;
}
Array2<TileStack>& MapLayer::tileStacks()
{
    return m_tileStacks;
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

const TileStack& MapLayer::at(int x, int y) const
{
    if(!isValid(x, y)) return m_emptyTileStack;
    return m_tileStacks(x, y);
}
TileStack& MapLayer::at(int x, int y)
{
    if(!isValid(x, y)) return m_emptyTileStack;
    return m_tileStacks(x, y);
}
const Tile& MapLayer::at(int x, int y, int z) const
{
    return at(x, y).at(z);
}
Tile& MapLayer::at(int x, int y, int z)
{
    return at(x, y).at(z);
}

std::vector<RectangleF> MapLayer::queryTileColliders(const RectangleF& queryRegion) const
{
    const Vec2F& queryRegionTopLeft     = queryRegion.min;
    const Vec2F& queryRegionBottomRight = queryRegion.max;
    int firstTileX = std::max(Util::fastFloor(queryRegionTopLeft.x / World::tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(queryRegionTopLeft.y / World::tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(queryRegionBottomRight.x / World::tileSize), m_width - 1);
    int lastTileY = std::min(Util::fastFloor(queryRegionBottomRight.y / World::tileSize), m_height - 1);

    std::vector<RectangleF> colliders;
    for(int x = firstTileX; x <= lastTileX; ++x)
    {
        for(int y = firstTileY; y <= lastTileY; ++y)
        {
            const TileStack& tileStack = at(x, y);
            for(const auto& tile : tileStack.tiles())
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
