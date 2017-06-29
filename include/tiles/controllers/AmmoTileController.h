#pragma once

#include "tiles/controllers/TileController.h"

#include "ResourceManager.h"

class ProjectilePrefab;

class AmmoTileController : public TileController
{
public:
    AmmoTileController(Tile& owner, ComponentCommonData* commonData);
    AmmoTileController(const AmmoTileController& other, Tile& owner);
    ~AmmoTileController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition) override;

    std::unique_ptr<TileController> clone(Tile& owner) const override;

private:
    TileAmmoGroupType m_ammoGroup;
    ResourceHandle<ProjectilePrefab> m_projectile;
};