#include "World.h"

#include "Root.h"

#include "Tile.h"
#include "TileStack.h"
#include "MapLayer.h"

#include "WindowSpaceManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/Util.h"

using namespace Geo;

World::World(Root& root) :
    m_root(root),
    m_width(worldWidth),
    m_height(worldHeight),
    m_mapLayer(std::make_unique<MapLayer>(*this, m_width, m_height)),
    m_camera(Vec2F(m_width* tileSize / 2.0f, m_height* tileSize / 2.0f), viewWidth* tileSize, viewHeight* tileSize),
    m_mapGenerator(m_width, m_height)
{
    m_mapGenerator.generate(*m_mapLayer);
}
World::~World()
{

}

void World::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_root.windowSpaceManager().setViewToRegion(WindowSpaceManager::Region::World, m_camera.asRect());

    Vec2F cameraTopLeft     = m_camera.position() - Vec2F(m_camera.width() / 2.0f, m_camera.height() / 2.0f);
    Vec2F cameraBottomRight = m_camera.position() + Vec2F(m_camera.width() / 2.0f, m_camera.height() / 2.0f);
    int firstTileX = std::max(Util::fastFloor(cameraTopLeft.x / tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(cameraTopLeft.y / tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(cameraBottomRight.x / tileSize) + 1, m_width - 1);
    int lastTileY = std::min(Util::fastFloor(cameraBottomRight.y / tileSize) + 1, m_height - 1);

    //x,y are inverted here because we want to draw top down
    for(int y = firstTileY; y <= lastTileY; ++y)
    {
        for(int x = firstTileX; x <= lastTileX; ++x)
        {
            const TileStack& tileStack = m_mapLayer->at(x, y);
            int z = 0;
            for(const auto& tile : tileStack.tiles())
            {
                tile->draw(renderTarget, renderStates, x, y, z, *m_mapLayer);
                ++z;
            }
        }
    }
}

void World::moveCamera(const Geo::Vec2F& displacement)
{
    m_camera.move(displacement);
}

const Camera& World::camera() const
{
    return m_camera;
}
int World::width() const
{
    return m_width;
}
int World::height() const
{
    return m_height;
}

const MapLayer& World::map() const
{
    return *m_mapLayer;
}
const MapGenerator& World::mapGenerator() const
{
    return m_mapGenerator;
}
