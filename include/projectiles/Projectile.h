#pragma once

#include "Configuration.h"

#include "ComponentFactory.h"

#include <memory>

#include "../LibS/Shapes.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class ProjectileModel;
class ProjectileRenderer;
class ProjectileController;
class ComponentCommonData;
class Entity;
class World;
class TileStack;
class EntityCollider;
class TileCollider;

class Projectile
{
public:
    Projectile(
        int id,
        const ComponentFactory<Projectile, ProjectileModel>& modelFac,
        const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac,
        const ComponentFactory<Projectile, ProjectileController>& controllerFac);
    Projectile(const Projectile& other);
    Projectile(Projectile&& other);
    Projectile& operator=(const Projectile& other);
    Projectile& operator=(Projectile&& other);
    ~Projectile();

    void loadFromConfiguration(ConfigurationNode& config);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const ProjectileModel& model() const;
    ProjectileModel& model();
    const ProjectileRenderer& renderer() const;
    ProjectileRenderer& renderer();
    const ProjectileController& controller() const;
    ProjectileController& controller();

    int id() const;

    void onProjectileCloned();
    void onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition);
    void onCollidedWithEntity(EntityCollider& entityCollider);
    void onCollidedWithTile(TileCollider& tileCollider);

    std::unique_ptr<Projectile> clone() const;
    std::unique_ptr<Projectile> instantiate(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) const;
protected:
    std::unique_ptr<ProjectileModel> m_model;
    std::unique_ptr<ProjectileRenderer> m_renderer;
    std::unique_ptr<ProjectileController> m_controller;
    int m_id;
};
