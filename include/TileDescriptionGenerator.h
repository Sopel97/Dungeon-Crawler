#pragma once

#include "TileDescription.h"

class Tile;

class TileDescriptionGenerator
{
private:
public:
    static TileDescription generate(const Tile& tile);
};