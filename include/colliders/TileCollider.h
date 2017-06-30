#pragma once

#include "../LibS/Shapes.h"

class Tile;

class TileCollider
{
private:
    ls::Rectangle2F m_volume;
    Tile* m_parent;

public:
    TileCollider(Tile& tile, const ls::Rectangle2F& volume);

    const ls::Rectangle2F& volume();
    ls::Rectangle2F boundingBox();
};