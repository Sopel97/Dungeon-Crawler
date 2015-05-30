#include "World.h"

#include "Root.h"

#include "MapLayer.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

World::World(Root& root) :
    m_root(root),
    m_mapLayer(std::make_unique<MapLayer>(*this, 256, 256))
{

}
World::~World()
{

}

void World::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{

}
