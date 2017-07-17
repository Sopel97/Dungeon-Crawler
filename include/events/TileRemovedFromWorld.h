#pragma once

#include "TileLocation.h"

class TileStack;

class TileRemovedFromWorld
{
public:
    const TileStack* tile;
    TileLocation location;
};