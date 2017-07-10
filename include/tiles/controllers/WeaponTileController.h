#pragma once

#include "tiles/controllers/TileController.h"

#include "projectiles/ProjectilePrefab.h"

#include "ResourceManager.h"

class WeaponTileController : public TileController
{
public:
    WeaponTileController(Tile& owner);
    WeaponTileController(const WeaponTileController& other, Tile& owner);
    ~WeaponTileController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    AttackResult attack(World& world, Player& player, const ls::Vec2F& hintedPosition) override;

    std::unique_ptr<TileController> clone(Tile& owner) const override;

private:
    bool m_requiresAmmo;
    int m_ammoPerAttack;
    float m_chanceToBreak;
    std::vector<TileAmmoGroupType> m_allowedAmmoGroups;
    ResourceHandle<ProjectilePrefab> m_projectile;
};