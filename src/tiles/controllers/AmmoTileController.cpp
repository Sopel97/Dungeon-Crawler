#include "tiles/controllers/AmmoTileController.h"

#include "tiles/Tile.h"

#include "projectiles/ProjectilePrefab.h"

#include "ResourceLoaders.h"

#include "World.h"
#include "Player.h"

REGISTER_TILE_CONTROLLER_TYPE(AmmoTileController)

AmmoTileController::AmmoTileController(Tile& owner) :
    TileController(owner),
    m_ammoGroup(),
    m_projectile(nullptr)
{

}

AmmoTileController::AmmoTileController(const AmmoTileController& other, Tile& owner) :
    TileController(other, owner),
    m_ammoGroup(other.m_ammoGroup),
    m_projectile(other.m_projectile)
{

}

AmmoTileController::~AmmoTileController()
{

}

void AmmoTileController::loadFromConfiguration(ConfigurationNode& config)
{
    m_ammoGroup = config["ammoGroup"].get<std::string>();
    m_projectile = ResourceManager::instance().get<ProjectilePrefab>(config["projectile"].get<std::string>());
}

void AmmoTileController::indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{
    world.spawnProjectile(m_projectile.get(), world, player.entity(), hintedPosition);
}
TileAmmoGroupType AmmoTileController::ammoGroup() const
{
    return m_ammoGroup;
}

std::unique_ptr<TileController> AmmoTileController::clone(Tile& owner) const
{
    return std::make_unique<AmmoTileController>(*this, owner);
}

