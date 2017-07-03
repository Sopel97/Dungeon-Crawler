#include "projectiles/controllers/StationaryProjectileController.h"

#include "ResourceLoaders.h"

REGISTER_PROJECTILE_CONTROLLER_TYPE(StationaryProjectileController);

StationaryProjectileController::StationaryProjectileController(Projectile& owner, ComponentCommonData* commonData) :
    ProjectileController(owner)
{

}
StationaryProjectileController::StationaryProjectileController(const StationaryProjectileController& other, Projectile& owner) :
    ProjectileController(other, owner)
{

}
StationaryProjectileController::~StationaryProjectileController()
{

}

std::unique_ptr<ProjectileController> StationaryProjectileController::clone(Projectile& owner) const
{
    return std::make_unique<StationaryProjectileController>(*this, owner);
}