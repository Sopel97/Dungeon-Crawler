#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "MapGenerator.h"

#include "Camera.h"

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

    void moveCamera(const Geo::Vec2F& displacement);

protected:
    Root& m_root;
    int m_width;
    int m_height;
    std::unique_ptr<MapLayer> m_mapLayer;
    Camera m_camera;
    MapGenerator m_mapGenerator;
};

#endif // WORLD_H
