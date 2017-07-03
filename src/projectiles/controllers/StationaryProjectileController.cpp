#include "projectiles/controllers/StationaryProjectileController.h"

#include "ResourceLoaders.h"

REGISTER_PROJECTILE_CONTROLLER_TYPE(StationaryProjectileController);

StationaryProjectileController::StationaryProjectileController(Projectile& owner, ComponentCommonData* commonData) :
    ProjectileController(owner),
    m_ticksLeft(10)
{

}
StationaryProjectileController::StationaryProjectileController(const StationaryProjectileController& other, Projectile& owner) :
    ProjectileController(other, owner),
    m_ticksLeft(other.m_ticksLeft)
{

}
StationaryProjectileController::~StationaryProjectileController()
{

}
void StationaryProjectileController::update(World& world, float dt)
{
    --m_ticksLeft;
    if (m_ticksLeft <= 0) m_owner->model().setHealth(0);
}

std::unique_ptr<ProjectileController> StationaryProjectileController::clone(Projectile& owner) const
{
    return std::make_unique<StationaryProjectileController>(*this, owner);
}