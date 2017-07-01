#include "ProjectileSystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "World.h"
#include "MapLayer.h"
#include "TileColumn.h"

#include "AggroGroup.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/controllers/EntityController.h"

#include "projectiles/ProjectilePrefab.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/controllers/ProjectileController.h"

#include "colliders/Collisions.h"
#include "colliders/EntityCollider.h"

#include "Camera.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "GameConstants.h"

using namespace ls;

ProjectileSystem::ProjectileSystem(World& world, EntitySystem& entitySystem) :
    m_world(&world),
    m_entitySystem(&entitySystem)
{

}

std::vector<Projectile*> ProjectileSystem::query(const Rectangle2F& rect)
{
    std::vector<Projectile*> projectilesInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for (std::unique_ptr<Projectile>& projectile : m_projectiles)
    {
        ProjectileCollider collider = projectile->model().collider();
        const float xDist = std::abs(rectCenter.x - collider.volume().origin.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - collider.volume().origin.y) - halfRectHeight;
        if (xDist < collider.volume().radius && yDist < collider.volume().radius)
        {
            projectilesInRegion.push_back(projectile.get());
        }
    }

    return projectilesInRegion;
}
std::vector<ProjectileCollider> ProjectileSystem::queryColliders(const ls::Rectangle2F& rect)
{
    std::vector<ProjectileCollider> collidersInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for (std::unique_ptr<Projectile>& projectile : m_projectiles)
    {
        ProjectileCollider collider = projectile->model().collider();
        const float xDist = std::abs(rectCenter.x - collider.volume().origin.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - collider.volume().origin.y) - halfRectHeight;
        if (xDist < collider.volume().radius && yDist < collider.volume().radius)
        {
            collidersInRegion.push_back(collider);
        }
    }

    return collidersInRegion;
}
const std::vector<std::unique_ptr<Projectile>>& ProjectileSystem::projectiles() const
{
    return m_projectiles;
}
Projectile& ProjectileSystem::projectile(int i)
{
    return *(m_projectiles[i]);
}
const Projectile& ProjectileSystem::projectile(int i) const
{
    return *(m_projectiles[i]);
}

Projectile& ProjectileSystem::addProjectile(std::unique_ptr<Projectile>&& newEntity)
{
    m_projectiles.emplace_back(std::move(newEntity));
    return *(m_projectiles.back());
}
Projectile& ProjectileSystem::spawnProjectile(const ProjectilePrefab& prefab, World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    return addProjectile(prefab.instantiate(world, parentEntity, hintedPosition));
}
void ProjectileSystem::removeProjectile(Projectile& projectileToRemove)
{
    m_projectiles.erase(std::find_if(m_projectiles.begin(), m_projectiles.end(), [&projectileToRemove](std::unique_ptr<Projectile>& ent) {return ent.get() == &projectileToRemove; }), m_projectiles.end());
}
bool ProjectileSystem::isDead(const Projectile& projectile) const
{
    return projectile.model().isDead();
}
void ProjectileSystem::update(float dt)
{
    for (auto& projectile : m_projectiles)
    {
        update(*projectile, dt);
    }

    removeDeadProjectiles();
}

std::vector<Projectile*> ProjectileSystem::getVisibleProjectiles(const Camera& camera)
{
    return query(camera.viewRectangle());
}
void ProjectileSystem::update(Projectile& projectile, float dt)
{
    projectile.controller().update(*m_world, dt);

    if (projectile.model().canCollideWithEntities()) resolveCollisionsWithEntities(projectile);
    if (projectile.model().canCollideWithTiles()) resolveCollisionsWithTiles(projectile);
}
void ProjectileSystem::removeDeadProjectiles()
{
    m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(), [this](std::unique_ptr<Projectile>& proj) {return isDead(*proj); }), m_projectiles.end());
}

void ProjectileSystem::resolveCollisionsWithEntities(Projectile& projectile)
{
    //TODO: some fancy sweep collision checking
    //TODO: maybe check line of sight if requested by projectile

    const AggroGroupId projectileGroup = projectile.model().group();

    ProjectileCollider collider = projectile.model().collider();
    const ls::Rectangle2F colliderBoundingBox = collider.boundingBox();
    auto entities = m_entitySystem->queryColliders(colliderBoundingBox);

    for (EntityCollider& entityCollider : entities)
    {
        if (isDead(projectile)) return;

        Entity& entity = entityCollider.entity();

        const AggroGroupId entityGroup = entity.model().group();
        if (!EntityGroupRelations::canCollide(entityGroup, projectileGroup)) continue;

        if (Collisions::collide(entityCollider, collider))
        {
            projectile.onCollidedWithEntity(entityCollider);
        }
    }
}
void ProjectileSystem::resolveCollisionsWithTiles(Projectile& projectile)
{
    //TODO: some fancy sweep collision checking
    ProjectileCollider collider = projectile.model().collider();
    const ls::Rectangle2F boundingBox = collider.boundingBox();

    auto tileColliders = m_world->queryTileColliders(boundingBox);

    for (auto& tileCollider : tileColliders)
    {
        if (isDead(projectile)) return;

        if (Collisions::collide(tileCollider, collider))
        {
            projectile.onCollidedWithTile(tileCollider);
        }
    }
}
