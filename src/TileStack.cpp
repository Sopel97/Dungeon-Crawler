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
