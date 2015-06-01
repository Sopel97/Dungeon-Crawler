#include "TileStack.h"

#include "Tile.h"

TileStack::TileStack() :
    m_lastTile(-1)
{

}

TileStack::~TileStack()
{
    for(auto tile : m_tiles)
    {
        delete tile;
    }
}

const Tile* TileStack::top() const
{
    return m_tiles[m_lastTile];
}
Tile* TileStack::top()
{
    return m_tiles[m_lastTile];
}

void TileStack::push(Tile* tile)
{
    m_tiles.push_back(tile);
    ++m_lastTile;
}

Tile* TileStack::pop()
{
    Tile* tile = m_tiles[m_lastTile];
    m_tiles.erase(m_tiles.begin() + m_lastTile);
    --m_lastTile;
    return tile;
}

void TileStack::deleteTop()
{
    delete pop();
}

const std::vector<Tile*> TileStack::tiles() const
{
    return m_tiles;
}
std::vector<Tile*> TileStack::tiles()
{
    return m_tiles;
}
