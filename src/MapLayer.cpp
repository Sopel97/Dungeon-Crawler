#include "MapLayer.h"

#include "TileStack.h"
#include "World.h"

MapLayer::MapLayer(World& world, int width, int height) :
    m_world(world),
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

int MapLayer::width() const
{
    return m_tileStacks.sizeX();
}
int MapLayer::height() const
{
    return m_tileStacks.sizeY();
}
