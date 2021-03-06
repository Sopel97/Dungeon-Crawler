#pragma once

#include "configuration/Configuration.h"

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
        const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac);
    Projectile(const Projectile& other);
    Projectile(Projectile&& other);
    Projectile& operator=(const Projectile& other);
    Projectile& operator=(Projectile&& other);
    ~Projectile();

    void loadFromConfiguration(ConfigurationNode& config);

    const ProjectileModel& model() const;
    ProjectileModel& model();
    const ProjectileRenderer& renderer() const;
    ProjectileRenderer& renderer();

    int id() const;

    void onProjectileCloned();
    void onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition);
    void onCollidedWithEntity(EntityCollider& entityCollider);
    void onCollidedWithTile(TileCollider& tileCollider);
    void onParentEntityDeleted(); 

    std::unique_ptr<Projectile> clone() const;
    std::unique_ptr<Projectile> instantiate(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) const;
protected:
    std::unique_ptr<ProjectileModel> m_model;
    std::unique_ptr<ProjectileRenderer> m_renderer;
    int m_id;
};
