#pragma once

#include "TileLocation.h"

class TileStack;

class TilePlacedInWorld
{
public:
    const TileStack* tile;
    TileLocation location;
};