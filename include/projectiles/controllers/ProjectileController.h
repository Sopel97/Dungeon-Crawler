#pragma once

#include "Configuration.h"

#include <memory>

#include "projectiles/ProjectileComponent.h"

#include "../LibS/Shapes.h"

class Projectile;
class World;

class ProjectileController : public ProjectileComponent<ProjectileController, Projectile>
{
public:
    ProjectileController(Projectile& owner);
    ProjectileController(const ProjectileController& other, Projectile& owner);
    ~ProjectileController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    virtual void update(World& world, float dt);

    std::unique_ptr<ProjectileController> clone(Projectile& owner) const override;
};
