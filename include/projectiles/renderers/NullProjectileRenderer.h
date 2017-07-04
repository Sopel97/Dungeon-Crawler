#pragma once 

#include "ComponentCommonData.h"
#include "projectiles/renderers/ProjectileRenderer.h"

#include <memory>

class Projectile;

class NullProjectileRenderer : public ProjectileRenderer
{
public:
    NullProjectileRenderer(Projectile& owner);
    NullProjectileRenderer(const NullProjectileRenderer& other, Projectile& owner);
    ~NullProjectileRenderer() override;

    std::unique_ptr<ProjectileRenderer> clone(Projectile& owner) const override;
};
