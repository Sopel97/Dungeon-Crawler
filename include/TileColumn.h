#ifndef TILECOLUMN_H
#define TILECOLUMN_H

#include <vector>

#include "../LibS/Geometry.h"

class TileStack;

class TileColumn
{
public:
    TileColumn();
    ~TileColumn();

    const TileStack& top() const;
    TileStack& top();

    void push(TileStack* tile);

    TileStack* releaseTop();

    void deleteTop();
    void deleteAt(size_t z);

    //TODO: remove, this should be handled internally by TileStack
    int insert(TileStack* tile, int count = -1); //returns number of inserted elements. -1 count means that it will insert all
    int insert(TileStack* tile, size_t slotId, int count = -1); //same as above

    int erase(TileStack* tile, int count = -1); //same as above
    int erase(TileStack* tile, size_t slotId, int count = -1); //same as above

    bool isValid(int z) const;
    //it is up to the user to ensure m_emptyTile is not modified
    const TileStack& at(int z) const;
    TileStack& at(int z);
    int size() const;

    bool isTall() const;

    bool hasCollider() const;
    const ls::Rectangle2F& collider() const;

    const std::vector<TileStack*>& tiles() const;

    int topZ() const;

protected:
    std::vector<TileStack*> m_tiles;
    int m_lastTile;

    static TileStack m_emptyTile;
private:
    static const ls::Rectangle2F m_defaultCollider;
};

#endif // TILECOLUMN_H
