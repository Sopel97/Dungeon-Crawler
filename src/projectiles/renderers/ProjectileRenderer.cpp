#include "projectiles/renderers/ProjectileRenderer.h"

ProjectileRenderer::ProjectileRenderer(Projectile& owner) :
    ProjectileComponent(owner)
{

}
ProjectileRenderer::ProjectileRenderer(const ProjectileRenderer& other, Projectile& owner) :
    ProjectileComponent(other, owner)
{

}
ProjectileRenderer::~ProjectileRenderer()
{

}

void ProjectileRenderer::draw(SpriteBatch& spriteBatch) const
{

}
void ProjectileRenderer::drawMeta(SpriteBatch& spriteBatch) const
{

}

std::unique_ptr<ProjectileRenderer> ProjectileRenderer::clone(Projectile& owner) const
{
    return std::make_unique<ProjectileRenderer>(*this, owner);
}
