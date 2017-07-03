#pragma once

#include "Configuration.h"

#include <memory>

#include "projectiles/controllers/ProjectileController.h"

#include "../LibS/Shapes.h"

class Projectile;
class World;

class StationaryProjectileController : public ProjectileController
{
public:
    StationaryProjectileController(Projectile& owner, ComponentCommonData* commonData);
    StationaryProjectileController(const StationaryProjectileController& other, Projectile& owner);
    ~StationaryProjectileController() override;

    std::unique_ptr<ProjectileController> clone(Projectile& owner) const override;
};
