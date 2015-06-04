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

constexpr int viewWidth = 15;
constexpr int viewHeight = 15;

World::World(Root& root) :
    m_root(root),
    m_width(256),
    m_height(256),
    m_mapLayer(std::make_unique<MapLayer>(*this, m_width, m_height)),
    m_camera(Vec2F(m_width*32.0f/2.0f, m_height*32.0f/2.0f), viewWidth*32.0f, viewHeight*32.0f),
    m_mapGenerator()
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
    int firstTileX = std::max(Util::fastFloor(cameraTopLeft.x/32.0f), 0);
    int firstTileY = std::max(Util::fastFloor(cameraTopLeft.y/32.0f), 0);
    int lastTileX = std::min(Util::fastFloor(cameraBottomRight.x/32.0f) + 1, m_width - 1);
    int lastTileY = std::min(Util::fastFloor(cameraBottomRight.y/32.0f) + 1, m_height - 1);

    for(int x = firstTileX; x <= lastTileX; ++x)
    {
        for(int y = firstTileY; y <= lastTileY; ++y)
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
