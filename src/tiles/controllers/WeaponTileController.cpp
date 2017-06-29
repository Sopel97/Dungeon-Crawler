#include "tiles/controllers/WeaponTileController.h"

#include "tiles/Tile.h"

#include "ResourceLoaders.h"

REGISTER_TILE_CONTROLLER_TYPE(WeaponTileController)

WeaponTileController::WeaponTileController(Tile& owner, ComponentCommonData* commonData) :
    TileController(owner),
    m_usesAmmo(false),
    m_ammoPerAttack(0),
    m_allowedAmmoGroups(),
    m_projectile(nullptr)
{

}

WeaponTileController::WeaponTileController(const WeaponTileController& other, Tile& owner) :
    TileController(other, owner),
    m_usesAmmo(other.m_usesAmmo),
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
    m_usesAmmo = config["requiresAmmo"].get<bool>();
    if (m_usesAmmo)
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

}

std::unique_ptr<TileController> WeaponTileController::clone(Tile& owner) const
{
    return std::make_unique<WeaponTileController>(*this, owner);
}

