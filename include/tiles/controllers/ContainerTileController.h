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

    void use(Player& player, const TileLocation& location) override;
    void use(Player& player, const InventorySlotView& slot) override;

    virtual std::unique_ptr<TileController> clone() const;
};

REGISTER_TILE_CONTROLLER_TYPE(ContainerTileController)

