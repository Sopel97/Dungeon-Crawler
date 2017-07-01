#pragma once

#include "tiles/controllers/TileController.h"

#include "projectiles/ProjectilePrefab.h"

#include "ResourceManager.h"

class WeaponTileController : public TileController
{
public:
    WeaponTileController(Tile& owner, ComponentCommonData* commonData);
    WeaponTileController(const WeaponTileController& other, Tile& owner);
    ~WeaponTileController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void attack(World& world, Player& player, const ls::Vec2F& hintedPosition) override;

    std::unique_ptr<TileController> clone(Tile& owner) const override;

private:
    bool m_usesAmmo;
    int m_ammoPerAttack;
    std::vector<TileAmmoGroupType> m_allowedAmmoGroups;
    ResourceHandle<ProjectilePrefab> m_projectile;
};