#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "../LibS/Array2.h"

#include "Tile.h"

#include "../LibS/GeometryLight.h"

class World;
class TileStack;

class MapLayer
{
public:
    MapLayer(World& world, int width, int height);
    ~MapLayer();

    const Array2<TileStack>& tileStacks() const;
    Array2<TileStack>& tileStacks();
    int width() const;
    int height() const;

    bool isValid(int x, int y) const;

    const TileStack& at(int x, int y) const;
    TileStack& at(int x, int y);
    const Tile& at(int x, int y, int z) const; //TODO: make them both return the same type because it is giving problems. same for methods in tilestack
                                               //(probably will only return reference and will require user to ensure that coordinates are valid)
                                               //(or will return the empty tile which can (but shouldn't) be modifyable)
    Tile* at(int x, int y, int z);

    std::vector<Geo::RectangleF> queryTileColliders(const Geo::RectangleF& queryRegion) const;

protected:
    World& m_world;
    int m_width;
    int m_height;
    Array2<TileStack> m_tileStacks;

    static const TileStack m_emptyTileStack;
    static const Tile m_emptyTile;
};

#endif // MAPLAYER_H
