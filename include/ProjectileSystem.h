#pragma once

#include <memory>

#include "../LibS/Geometry.h"

#include "projectiles/Projectile.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;
class Camera;
class World;
class ProjectilePrefab;
class EntitySystem;

class ProjectileSystem
{
public:
    ProjectileSystem(World& world, EntitySystem& entitySystem);

    std::vector<Projectile*> query(const ls::Rectangle2F& rect);
    const std::vector<std::unique_ptr<Projectile>>& projectiles() const;

    Projectile& projectile(int i);
    const Projectile& projectile(int i) const;

    Projectile& addProjectile(std::unique_ptr<Projectile>&& newProjectile);
    Projectile& spawnProjectile(const ProjectilePrefab& prefab, World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition);
    void removeProjectile(Projectile& projectileToRemove);

    void update(float dt);

    std::vector<Projectile*> getVisibleProjectiles(const Camera& camera);
protected:
    World* m_world;
    EntitySystem* m_entitySystem;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;

    bool isDead(const Projectile& projectile) const;
    void update(Projectile& projectile, float dt);
    void removeDeadProjectiles();
    void resolveCollisionsWithEntities(Projectile& projectile);
    void resolveCollisionsWithTiles(Projectile& projectile);
};
