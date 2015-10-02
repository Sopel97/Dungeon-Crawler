#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "../LibS/Array2.h"

#include "tiles/Tile.h"

#include "../LibS/Geometry.h"

class World;
class TileColumn;

class MapLayer
{
public:
    MapLayer(World& world, int width, int height);
    ~MapLayer();

    const Array2<TileColumn>& tileStacks() const;
    Array2<TileColumn>& tileStacks();
    int width() const;
    int height() const;

    bool isValid(int x, int y) const;

    //it is up to the user to ensure m_emptyTileStack is not modyfied
    const TileColumn& at(int x, int y) const;
    TileColumn& at(int x, int y);
    const Tile& at(int x, int y, int z) const;
    Tile& at(int x, int y, int z);

    void placeTile(Tile* tile, int x, int y);
    Tile* takeTile(int x, int y);
    void deleteTile(int x, int y);


    std::vector<ls::RectangleF> queryTileColliders(const ls::RectangleF& queryRegion) const;

protected:
    World& m_world;
    int m_width;
    int m_height;
    Array2<TileColumn> m_tileColumns;

    static TileColumn m_emptyTileColumn;
};

#endif // MAPLAYER_H
