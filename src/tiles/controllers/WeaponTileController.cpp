#include "tiles/controllers/WeaponTileController.h"

#include "tiles/Tile.h"

#include "ResourceLoaders.h"

#include "Player.h"
#include "World.h"

#include "Rng.h"

REGISTER_TILE_CONTROLLER_TYPE(WeaponTileController)

WeaponTileController::WeaponTileController(Tile& owner) :
    TileController(owner),
    m_requiresAmmo(false),
    m_ammoPerAttack(0),
    m_chanceToBreak(0.0f),
    m_allowedAmmoGroups(),
    m_projectile(nullptr)
{

}

WeaponTileController::WeaponTileController(const WeaponTileController& other, Tile& owner) :
    TileController(other, owner),
    m_requiresAmmo(other.m_requiresAmmo),
    m_ammoPerAttack(other.m_ammoPerAttack),
    m_chanceToBreak(other.m_chanceToBreak),
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
        m_chanceToBreak = config["chanceToBreak"].getDefault<float>(0.0f);
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

        if (Rng<std::ranlux48>::instance().doesHappen(m_chanceToBreak))
        {
            // TODO: better way to do this
            player.weapon().erase(1);
        }
    }
}

std::unique_ptr<TileController> WeaponTileController::clone(Tile& owner) const
{
    return std::make_unique<WeaponTileController>(*this, owner);
}

