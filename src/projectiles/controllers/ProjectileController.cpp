#include "projectiles/controllers/ProjectileController.h"

ProjectileController::ProjectileController(Projectile& owner) :
    ProjectileComponent(owner)
{

}
ProjectileController::ProjectileController(const ProjectileController& other, Projectile& owner) :
    ProjectileComponent(other, owner)
{

}
ProjectileController::~ProjectileController()
{

}

void ProjectileController::loadFromConfiguration(ConfigurationNode& config)
{

}

void ProjectileController::update(World& world, float dt)
{

}

std::unique_ptr<ProjectileController> ProjectileController::clone(Projectile& owner) const
{
    return std::make_unique<ProjectileController>(*this, owner);
}
