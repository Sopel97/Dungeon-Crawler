#ifndef PLAINTILECONTROLLER_H
#define PLAINTILECONTROLLER_H

#include "TileController.h"

#include <memory>

class Tile;

class PlainTileController : public TileController
{
public:
    PlainTileController(Tile* owner);
    PlainTileController(const PlainTileController& other);
    virtual ~PlainTileController();

    virtual std::unique_ptr<TileController> clone() const;
};

#endif // PLAINTILECONTROLLER_H
