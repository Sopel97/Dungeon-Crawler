#include "TileStack.h"

#include "Tile.h"

#include "TileModel.h"
#include "TileView.h"
#include "TileController.h"

#include "../LibS/make_unique.h"

const Tile TileStack::m_emptyTile {std::make_unique<TileModel>(nullptr), std::make_unique<TileView>(nullptr), std::make_unique<TileController>(nullptr)};

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

const std::vector<Tile*>& TileStack::tiles() const
{
    return m_tiles;
}

bool TileStack::isValid(int z) const
{
    return m_lastTile >= z;
}

const Tile& TileStack::at(int z) const
{
    if(isValid(z)) return *(m_tiles[z]);
    return m_emptyTile;
}
Tile* TileStack::at(int z)
{
    if(isValid(z)) return m_tiles[z];
    return nullptr;
}
int TileStack::size() const
{
    return m_lastTile + 1;
}

bool TileStack::isTall() const
{
    for(const Tile* tile : m_tiles)
        if(tile->view().isTall()) return true;

    return false;
}
