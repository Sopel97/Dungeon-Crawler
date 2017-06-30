#include "ProjectileSystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "World.h"
#include "MapLayer.h"
#include "TileColumn.h"

#include "projectiles/ProjectilePrefab.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/controllers/ProjectileController.h"

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
        const ls::Circle2F collider = projectile->model().collider();
        const float xDist = std::abs(rectCenter.x - collider.origin.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - collider.origin.y) - halfRectHeight;
        if (xDist < collider.radius && yDist < collider.radius)
        {
            projectilesInRegion.push_back(projectile.get());
        }
    }

    return projectilesInRegion;
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

void ProjectileSystem::addProjectile(std::unique_ptr<Projectile>&& newEntity)
{
    m_projectiles.emplace_back(std::move(newEntity));
}
void ProjectileSystem::spawnProjectile(const ProjectilePrefab& prefab, World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    addProjectile(prefab.instantiate(world, parentEntity, hintedPosition));
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
    //TODO: update projectiles, check collisions with tiles, check collisions with entities
}

std::vector<Projectile*> ProjectileSystem::getVisibleProjectiles(const Camera& camera)
{
    return query(camera.viewRectangle());
}
