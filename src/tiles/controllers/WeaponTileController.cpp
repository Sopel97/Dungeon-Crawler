#include "tiles/controllers/WeaponTileController.h"

#include "tiles/Tile.h"

#include "ResourceLoaders.h"

#include "Player.h"
#include "World.h"

REGISTER_TILE_CONTROLLER_TYPE(WeaponTileController)

WeaponTileController::WeaponTileController(Tile& owner) :
    TileController(owner),
    m_requiresAmmo(false),
    m_ammoPerAttack(0),
    m_allowedAmmoGroups(),
    m_projectile(nullptr)
{

}

WeaponTileController::WeaponTileController(const WeaponTileController& other, Tile& owner) :
    TileController(other, owner),
    m_requiresAmmo(other.m_requiresAmmo),
    m_ammoPerAttack(other.m_ammoPerAttack),
    m_allowedAmmoGroups(other.m_allowedAmmoGroups),
    m_projectile(other.m_projectile)
{

}

WeaponTileController::~WeaponTileController()
{

}

void WeaponTileController::loadFromConfiguration(ConfigurationNode& config)
{
    m_requiresAmmo = config["requiresAmmo"].get<bool>();
    if (m_requiresAmmo)
    {
        m_ammoPerAttack = config["ammoPerAttack"].get<int>();
        ConfigurationNode allowedAmmoGroupsConfig = config["allowedAmmoGroups"];
        const int numAllowedAmmoGroups = allowedAmmoGroupsConfig.length();
        for (int i = 1; i <= numAllowedAmmoGroups; ++i)
        {
            m_allowedAmmoGroups.emplace_back(allowedAmmoGroupsConfig[i].get<std::string>());
        }
    }
    else
    {
        m_projectile = ResourceManager::instance().get<ProjectilePrefab>(config["projectile"].get<std::string>());
    }
}

void WeaponTileController::attack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{
    if (m_requiresAmmo)
    {
        TileStack& ammo = player.ammo();
        if (ammo.isEmpty()) return;
        if (ammo.quantity() < m_ammoPerAttack) return;

        ammo.tile().controller().indirectAttack(world, player, hintedPosition);
        ammo.erase(m_ammoPerAttack);
    }
    else
    {
        world.spawnProjectile(m_projectile.get(), world, player.entity(), hintedPosition);
    }
}

std::unique_ptr<TileController> WeaponTileController::clone(Tile& owner) const
{
    return std::make_unique<WeaponTileController>(*this, owner);
}

