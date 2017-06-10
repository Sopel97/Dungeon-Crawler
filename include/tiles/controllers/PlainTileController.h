#ifndef PLAINTILECONTROLLER_H
#define PLAINTILECONTROLLER_H

#include "TileController.h"

#include "ResourceLoaders.h"

#include <memory>

class Tile;

class PlainTileController : public TileController
{
public:
    PlainTileController();
    PlainTileController(const PlainTileController& other);
    ~PlainTileController() override;

    std::unique_ptr<TileController> clone() const override;
};

REGISTER_TILE_CONTROLLER_TYPE(PlainTileController)

#endif // PLAINTILECONTROLLER_H
