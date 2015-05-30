#ifndef PLAINTILECONTROLLER_H
#define PLAINTILECONTROLLER_H

#include "TileController.h"

class Tile;

class PlainTileController : public TileController
{
public:
    PlainTileController(Tile* owner);
    virtual ~PlainTileController();
};

#endif // PLAINTILECONTROLLER_H
