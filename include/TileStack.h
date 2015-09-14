#ifndef TILESTACK_H
#define TILESTACK_H

#include <vector>

#include "../LibS/Geometry.h"

class Tile;

class TileStack
{
public:
    TileStack();
    ~TileStack();

    const Tile& top() const;
    Tile& top();

    void push(Tile* tile);

    Tile* pop(); //also returns ownership to the tile

    void deleteTop(); //this one deletes the tile on top

    int insert(Tile* tile, int count = -1); //returns number of inserted elements. -1 count means that it will insert all
    int insert(Tile* tile, int slotId, int count = -1); //same as above

    int erase(Tile* tile, int count = -1); //same as above
    int erase(Tile* tile, int slotId, int count = -1); //same as above

    bool isValid(int z) const;
    //it is up to the user to ensure m_emptyTile is not modyfied
    const Tile& at(int z) const;
    Tile& at(int z);
    int size() const;

    bool isTall() const;

    bool hasCollider() const;
    const ls::RectangleF& collider() const;

    const std::vector<Tile*>& tiles() const;

    int topZ() const;

protected:
    std::vector<Tile*> m_tiles;
    int m_lastTile;

    static Tile m_emptyTile;
private:
    static const ls::RectangleF m_defaultCollider;
};

#endif // TILESTACK_H
