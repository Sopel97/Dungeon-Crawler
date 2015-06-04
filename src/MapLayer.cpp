#include "MapLayer.h"

#include "TileStack.h"
#include "World.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileController.h"

TileStack MapLayer::m_emptyTileStack {};
Tile MapLayer::m_emptyTile {std::make_unique<TileModel>(nullptr), std::make_unique<TileView>(nullptr), std::make_unique<TileController>(nullptr)};

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
    //this one CANNOT return empty tile because it could be modified outside which is bad
    //but will ALWAYS return some TileStack so it is up to the user to pass valid x,y coordinates
    return m_tileStacks(x, y);
}
const Tile* MapLayer::at(int x, int y, int z) const
{
    const TileStack& tileStack = at(x, y);
    if(tileStack.isValid(z)) return tileStack.at(z);

    return &m_emptyTile;
}
Tile* MapLayer::at(int x, int y, int z)
{
    TileStack& tileStack = at(x, y);
    if(tileStack.isValid(z)) return tileStack.at(z);

    return nullptr;
}
