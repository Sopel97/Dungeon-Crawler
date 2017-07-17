#ifndef TILECOLUMN_H
#define TILECOLUMN_H

#include <vector>
#include <memory>

#include "colliders/TileCollider.h"

#include "../LibS/Geometry.h"

class TileStack;

class TileColumn
{
public:
    const TileStack& top() const;
    TileStack& top();

    TileStack& placeOnTop(TileStack&& tile);

    TileStack takeFromTop();
    TileStack take(int z);
        
    bool isValid(int z) const;
    //it is up to the user to ensure m_emptyTile is not modified
    const TileStack& at(int z) const;
    TileStack& at(int z);
    int size() const;

    bool isTall() const;

    bool hasCollider() const;
    TileCollider collider(const ls::Vec2I& pos);

    const std::vector<TileStack>& tiles() const;
    std::vector<TileStack>& tiles();

    int topZ() const;

protected:
    std::vector<TileStack> m_tiles;

    static TileStack m_emptyTile;
};

#endif // TILECOLUMN_H
