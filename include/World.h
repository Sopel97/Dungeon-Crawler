#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "MapGenerator.h"

#include "Camera.h"

#include "EntitySystem.h"

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Root;
class MapLayer;

class World
{
public:
    World(Root& root);
    ~World();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawOuterBorder(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map);

    void moveCamera(const Geo::Vec2F& displacement);

    const Camera& camera() const;
    int width() const;
    int height() const;

    const MapLayer& map() const;
    const EntitySystem& entitySystem() const;
    const MapGenerator& mapGenerator() const;

    Geo::Vec2I worldToTile(const Geo::Vec2F& position) const;

    void update(float dt);

    float drag(const Geo::Vec2F& position) const;

    std::vector<Geo::RectangleF> queryTileColliders(const Geo::RectangleF& queryRegion) const;
protected:
    Root& m_root;
    int m_width;
    int m_height;
    std::unique_ptr<MapLayer> m_mapLayer;
    EntitySystem m_entitySystem;
    Camera m_camera;
    MapGenerator m_mapGenerator;
    Entity* m_playerEntity;

    static constexpr int viewWidth = 15;
    static constexpr int viewHeight = 15;
    static constexpr int worldWidth = 128;
    static constexpr int worldHeight = 128;
};

#endif // WORLD_H
