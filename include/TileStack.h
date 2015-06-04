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

    bool isValid(int z) const;
    const Tile& at(int z) const;
    Tile* at(int z);
    int size() const;

    const std::vector<Tile*>& tiles() const;

protected:
    std::vector<Tile*> m_tiles;
    int m_lastTile;

    static Tile m_emptyTile;
};

#endif // TILESTACK_H
