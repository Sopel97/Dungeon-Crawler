#include "World.h"

#include "Root.h"

#include "Tile.h"
#include "TileView.h"
#include "TileModel.h"
#include "TileController.h"

#include "Entity.h"
#include "EntityModel.h"
#include "EntityView.h"
#include "EntityController.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "WindowSpaceManager.h"

#include "TallDrawable.h"
#include "TallEntityDrawable.h"
#include "TallTileStackDrawable.h"

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
    m_playerEntity = root.player().createPlayerEntity();
    m_entitySystem.addEntity(m_playerEntity, m_camera.position());
}
World::~World()
{

}

void World::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_root.windowSpaceManager().setViewToRegion(WindowSpaceManager::Region::World, m_camera.viewRectangle());

    const RectangleF cameraRect = m_camera.viewRectangle();
    const Vec2F& cameraTopLeft     = cameraRect.min;
    const Vec2F& cameraBottomRight = cameraRect.max;
    int firstTileX = std::max(Util::fastFloor(cameraTopLeft.x / tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(cameraTopLeft.y / tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(cameraBottomRight.x / tileSize) + 1, m_width - 1);
    int lastTileY = std::min(Util::fastFloor(cameraBottomRight.y / tileSize) + 1, m_height - 1);

    std::vector<TallDrawable*> tallDrawables;

    //x,y are inverted here because we want to draw top down
    for(int y = firstTileY; y <= lastTileY; ++y)
    {
        for(int x = firstTileX; x <= lastTileX; ++x)
        {
            const TileStack& tileStack = m_mapLayer->at(x, y);
            int z = 0;
            for(const auto& tile : tileStack.tiles())
            {
                if(tile->view().isTall())
                {
                    tallDrawables.push_back(new TallTileStackDrawable(tileStack, x, y, *m_mapLayer));
                    break;
                }
                if(z == 0)
                {
                    if(tile->view().coversOuterBorders())
                    {
                        drawOuterBorder(renderTarget, renderStates, x, y, *m_mapLayer);
                        tile->draw(renderTarget, renderStates, x, y, z, *m_mapLayer);
                    }
                    else
                    {
                        tile->draw(renderTarget, renderStates, x, y, z, *m_mapLayer);
                        drawOuterBorder(renderTarget, renderStates, x, y, *m_mapLayer);
                    }
                }
                else
                {
                    tile->draw(renderTarget, renderStates, x, y, z, *m_mapLayer);
                }

                ++z;
            }
        }
    }

    for(Entity* visibleEntity : m_entitySystem.getVisibleEntities(m_camera))
    {
        tallDrawables.push_back(new TallEntityDrawable(visibleEntity));
    }


    for(auto& tallDrawable : tallDrawables) //TODO: sorting before this
    {
        tallDrawable->draw(renderTarget, renderStates);
    }
}

void World::drawOuterBorder(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map)
{
    auto areTilesEqual = [](const Tile * lhs, const Tile * rhs)->bool {return lhs->id() == rhs->id();};
    auto borderPriorityCompare = [](const Tile * lhs, const Tile * rhs)->bool {return lhs->view().outerBorderPriority() < rhs->view().outerBorderPriority();};
    std::vector<const Tile*> differentNeigbourTiles;
    int thisTileOuterBorderPriority = map.at(x, y, 0).view().outerBorderPriority();
    for(int xoffset = -1; xoffset <= 1; ++xoffset)
    {
        for(int yoffset = -1; yoffset <= 1; ++yoffset)
        {
            if(xoffset == 0 && yoffset == 0) continue;
            int xx = x + xoffset;
            int yy = y + yoffset;
            const Tile& tile = map.at(xx, yy, 0);
            if(!tile.view().hasOuterBorder() || tile.view().outerBorderPriority() <= thisTileOuterBorderPriority) continue;

            bool firstSuchNeighbour = true;
            for(const auto& neighbour : differentNeigbourTiles)
            {
                if(areTilesEqual(&tile, neighbour))
                {
                    firstSuchNeighbour = false;
                    break;
                }
            }
            if(firstSuchNeighbour)
            {
                differentNeigbourTiles.push_back(&tile);
            }
        }
    }
    std::sort(differentNeigbourTiles.begin(), differentNeigbourTiles.end(), borderPriorityCompare);

    for(const auto& neighbour : differentNeigbourTiles)
    {
        neighbour->drawOutside(renderTarget, renderStates, x, y, 0, map);
    }
}

void World::moveCamera(const Vec2F& displacement)
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
const EntitySystem& World::entitySystem() const
{
    return m_entitySystem;
}
const MapGenerator& World::mapGenerator() const
{
    return m_mapGenerator;
}

Vec2I World::worldToTile(const Vec2F& position) const
{
    return Vec2I(position.x / tileSize, position.y / tileSize);
}

void World::update(float dt)
{
    m_entitySystem.updateEntities(this, dt);

    m_camera.setPosition(m_playerEntity->model().position());
}

float World::drag(const Vec2F& position) const
{
    Vec2I tilePosition = worldToTile(position);
    const TileStack& tileStack = m_mapLayer->at(tilePosition.x, tilePosition.y);
    const Tile& tile = tileStack.at(0);
    return tile.model().drag();
}

std::vector<RectangleF> World::queryTileColliders(const RectangleF& queryRegion) const
{
    return m_mapLayer->queryTileColliders(queryRegion);
}
