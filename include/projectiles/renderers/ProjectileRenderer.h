#pragma once 

#include "Configuration.h"

#include "ComponentCommonData.h"
#include "projectiles/ProjectileComponent.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Projectile;

class ProjectileRenderer : public ProjectileComponent<ProjectileRenderer, Projectile>
{
public:
    ProjectileRenderer(Projectile& owner);
    ProjectileRenderer(const ProjectileRenderer& other, Projectile& owner);
    ~ProjectileRenderer() override;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    std::unique_ptr<ProjectileRenderer> clone(Projectile& owner) const override;
};
