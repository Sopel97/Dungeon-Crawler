#include "MapLayer.h"

#include "TileStack.h"
#include "World.h"

TileStack MapLayer::m_emptyTileStack{};

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

bool MapLayer::inWorldBounds(int x, int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

const TileStack& MapLayer::at(int x, int y) const
{
    if(!inWorldBounds(x, y)) return m_emptyTileStack;
    return m_tileStacks(x, y);
}
TileStack& MapLayer::at(int x, int y)
{
    //this one CANNOT return empty tile because it could be modified outside which is bad
    return m_tileStacks(x, y);
}
