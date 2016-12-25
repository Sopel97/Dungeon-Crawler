#pragma once

#include "TileController.h"

#include "ResourceLoaders.h"

class Tile;

class ContainerTileController : public TileController
{
public:
    ContainerTileController();
    ContainerTileController(const ContainerTileController& other);
    virtual ~ContainerTileController();

    virtual void onTileUsedByPlayer(const TileLocation& location);

    virtual std::unique_ptr<TileController> clone() const;
};

REGISTER_TILE_CONTROLLER_TYPE(ContainerTileController)

