#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "../LibS/Array2.h"

class World;
class TileStack;

class MapLayer
{
public:
    MapLayer(World& world, int width, int height);
    ~MapLayer();

    const Array2<TileStack>& tileStacks() const;
    int width() const;
    int height() const;
protected:
    World& m_world;
    Array2<TileStack> m_tileStacks;
};

#endif // MAPLAYER_H
