#ifndef TILESTACK_H
#define TILESTACK_H

#include <vector>

class Tile;

class TileStack
{
public:
    TileStack();
    ~TileStack();

    const Tile* top() const;
    Tile* top();

    void push(Tile* tile);

    Tile* pop(); //also returns ownership to the tile

    void deleteTop(); //this one deletes the tile on top

    const std::vector<Tile*> tiles() const;
    std::vector<Tile*> tiles();

protected:
    std::vector<Tile*> m_tiles;
    int m_lastTile;
};

#endif // TILESTACK_H
