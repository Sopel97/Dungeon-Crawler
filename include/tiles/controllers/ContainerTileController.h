#pragma once

#include "TileController.h"

#include "ResourceLoaders.h"

class Tile;

class ContainerTileController : public TileController
{
public:
    ContainerTileController(Tile* owner);
    ContainerTileController(const ContainerTileController& other);
    virtual ~ContainerTileController();

    virtual void onTileUsedByPlayer(const TileLocation& location);

    virtual std::unique_ptr<TileController> clone() const;
    virtual std::unique_ptr<TileController> create(Tile* owner) const;
};

REGISTER_TILE_CONTROLLER_TYPE(ContainerTileController)

