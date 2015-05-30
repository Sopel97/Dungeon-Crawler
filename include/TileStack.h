#ifndef TILESTACK_H
#define TILESTACK_H

#include <vector>

class Tile;

class TileStack
{
public:
    TileStack();
    ~TileStack();
protected:
    std::vector<Tile*> m_tiles;
    int m_lastTile;
};

#endif // TILESTACK_H
