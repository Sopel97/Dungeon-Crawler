#ifndef PLAINTILECONTROLLER_H
#define PLAINTILECONTROLLER_H

#include "TileController.h"

#include "ResourceLoaders.h"

#include <memory>

class Tile;

class PlainTileController : public TileController
{
public:
    PlainTileController(Tile& owner);
    PlainTileController(const PlainTileController& other, Tile& owner);
    ~PlainTileController() override;

    std::unique_ptr<TileController> clone(Tile& owner) const override;
};

#endif // PLAINTILECONTROLLER_H
