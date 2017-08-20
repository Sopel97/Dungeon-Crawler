#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "colliders/TileCollider.h"

#include "../LibS/Array2.h"

#include "../LibS/Shapes.h"

class World;
class TileColumn;
class TileStack;
class Tile;
class TileLocation;

class MapLayer
{
public:
    MapLayer(World& world, int width, int height);

    const ls::Array2<TileColumn>& tileColumns() const;
    int width() const;
    int height() const;

    bool isValid(int x, int y) const;

    //it is up to the user to ensure m_emptyTileStack is not modified
    const TileColumn& at(int x, int y) const;
    TileColumn& at(int x, int y);
    const TileStack& at(int x, int y, int z) const;
    TileStack& at(int x, int y, int z);

    void placeTile(TileStack&& tileStack, int x, int y);
    void placeTileMerge(TileStack&& tileStack, int x, int y);
    TileStack takeTile(int x, int y);

    void addTiles(int x, int y, int z, int count);
    void removeTiles(int x, int y, int z, int count);
    TileStack splitTiles(int x, int y, int z, int count);

    std::vector<TileCollider> queryTileColliders(const ls::Rectangle2I& queryRegion);
    std::vector<ls::Rectangle2F> queryLightOccluders(const ls::Rectangle2I& queryRegion);

protected:
    World& m_world;
    int m_width;
    int m_height;
    ls::Array2<TileColumn> m_tileColumns;

    static TileColumn m_emptyTileColumn;

    void onTilePlaced(TileStack& stack, int x, int y, int z);
    void onTileRemoved(TileStack& stack, int x, int y, int z);

    using TileUpdateFunction = void (Tile::*)(const TileLocation&, const ls::Vec2I&);
    void updateNearbyTiles(int x, int y, int z, TileUpdateFunction func);
};

#endif // MAPLAYER_H
