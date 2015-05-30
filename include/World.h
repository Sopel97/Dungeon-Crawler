#ifndef WORLD_H
#define WORLD_H

#include <memory>

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

protected:
    Root& m_root;
    std::unique_ptr<MapLayer> m_mapLayer;
    //TODO: do proper camera before anything else
};

#endif // WORLD_H
