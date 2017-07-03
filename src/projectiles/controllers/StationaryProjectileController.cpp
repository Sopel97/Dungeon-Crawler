#include "projectiles/controllers/StationaryProjectileController.h"

#include "ResourceLoaders.h"

REGISTER_PROJECTILE_CONTROLLER_TYPE(StationaryProjectileController);

StationaryProjectileController::StationaryProjectileController(Projectile& owner, ComponentCommonData* commonData) :
    ProjectileController(owner),
    m_timeLeft(1.0f)
{

}
StationaryProjectileController::StationaryProjectileController(const StationaryProjectileController& other, Projectile& owner) :
    ProjectileController(other, owner),
    m_timeLeft(other.m_timeLeft)
{

}
StationaryProjectileController::~StationaryProjectileController()
{

}
void StationaryProjectileController::loadFromConfiguration(ConfigurationNode& config)
{
    m_timeLeft = config["lifetime"].get<float>();
}
void StationaryProjectileController::update(World& world, float dt)
{
    m_timeLeft -= dt;
    if (m_timeLeft <= 0.0f) m_owner->model().setHealth(0);
}

std::unique_ptr<ProjectileController> StationaryProjectileController::clone(Projectile& owner) const
{
    return std::make_unique<StationaryProjectileController>(*this, owner);
}