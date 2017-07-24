#include "projectiles/ProjectilePrefab.h"

#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/Projectile.h"

int ProjectilePrefab::m_lastId = -1;

ProjectilePrefab::ProjectilePrefab(
    const ComponentFactory<Projectile, ProjectileModel>& modelFac,
    const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac) :
    m_projectile(std::make_unique<Projectile>(++m_lastId, modelFac, rendererFac)),
    m_modelFactory(&modelFac),
    m_rendererFactory(&rendererFac)
{
}

ProjectilePrefab::~ProjectilePrefab()
{
}

void ProjectilePrefab::loadFromConfiguration(ConfigurationNode& config)
{
    m_projectile->loadFromConfiguration(config);
}

std::unique_ptr<Projectile> ProjectilePrefab::instantiate(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) const
{
    return m_projectile->instantiate(world, parentEntity, hintedPosition);
}
