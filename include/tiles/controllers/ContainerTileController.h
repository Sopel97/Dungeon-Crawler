#pragma once

#include "TileController.h"

#include "ResourceLoaders.h"

class Tile;

class ContainerTileController : public TileController
{
public:
    ContainerTileController(Tile& owner, ComponentCommonData* commonData);
    ContainerTileController(const ContainerTileController& other, Tile& owner);
    ~ContainerTileController() override;

    void use(Player& player, const TileLocation& location) override;
    void use(Player& player, const InventorySlotView& slot) override;

    std::unique_ptr<TileController> clone(Tile& owner) const override;
};

