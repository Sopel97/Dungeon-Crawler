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

void ProjectileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}
void ProjectileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}

std::unique_ptr<ProjectileRenderer> ProjectileRenderer::clone(Projectile& owner) const
{
    return std::make_unique<ProjectileRenderer>(*this, owner);
}
