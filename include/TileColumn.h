#ifndef TILECOLUMN_H
#define TILECOLUMN_H

#include <vector>
#include <memory>

#include "../LibS/Geometry.h"

class TileStack;

class TileColumn
{
public:
    const TileStack& top() const;
    TileStack& top();

    void placeOnTop(TileStack&& tile);

    TileStack takeFromTop();
        
    bool isValid(int z) const;
    //it is up to the user to ensure m_emptyTile is not modified
    const TileStack& at(int z) const;
    TileStack& at(int z);
    int size() const;

    bool isTall() const;

    bool hasCollider() const;
    const ls::Rectangle2F& collider() const;

    const std::vector<TileStack>& tiles() const;

    int topZ() const;

protected:
    std::vector<TileStack> m_tiles;

    static TileStack m_emptyTile;
private:
    static const ls::Rectangle2F m_defaultCollider;
};

#endif // TILECOLUMN_H
