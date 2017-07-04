#include "projectiles/renderers/NullProjectileRenderer.h"

#include "ResourceLoaders.h"

REGISTER_PROJECTILE_RENDERER_TYPE(NullProjectileRenderer);

NullProjectileRenderer::NullProjectileRenderer(Projectile& owner) :
    ProjectileRenderer(owner)
{

}
NullProjectileRenderer::NullProjectileRenderer(const NullProjectileRenderer& other, Projectile& owner) :
    ProjectileRenderer(other, owner)
{

}
NullProjectileRenderer::~NullProjectileRenderer()
{

}

std::unique_ptr<ProjectileRenderer> NullProjectileRenderer::clone(Projectile& owner) const
{
    return std::make_unique<NullProjectileRenderer>(*this, owner);
}