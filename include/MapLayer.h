#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "colliders/TileCollider.h"

#include "../LibS/Array2.h"

#include "../LibS/Geometry.h"

class World;
class TileColumn;
class TileStack;

class MapLayer
{
public:
    MapLayer(World& world, int width, int height);

    const ls::Array2<TileColumn>& tileColumns() const;
    ls::Array2<TileColumn>& tileColumns();
    int width() const;
    int height() const;

    bool isValid(int x, int y) const;

    //it is up to the user to ensure m_emptyTileStack is not modified
    const TileColumn& at(int x, int y) const;
    TileColumn& at(int x, int y);
    const TileStack& at(int x, int y, int z) const;
    TileStack& at(int x, int y, int z);

    void placeTile(TileStack&& tile, int x, int y);
    TileStack takeTile(int x, int y);


    std::vector<TileCollider> queryTileColliders(const ls::Rectangle2F& queryRegion);

protected:
    World& m_world;
    int m_width;
    int m_height;
    ls::Array2<TileColumn> m_tileColumns;

    static TileColumn m_emptyTileColumn;
};

#endif // MAPLAYER_H
