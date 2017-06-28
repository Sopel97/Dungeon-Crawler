#include "projectiles/ProjectilePrefab.h"

#include "projectiles/controllers/ProjectileController.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/Projectile.h"

int ProjectilePrefab::m_lastId = -1;

ProjectilePrefab::ProjectilePrefab(
    const ComponentFactory<Projectile, ProjectileModel>& modelFac,
    const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac,
    const ComponentFactory<Projectile, ProjectileController>& controllerFac) :
    m_projectile(std::make_unique<Projectile>(++m_lastId, modelFac, rendererFac, controllerFac)),
    m_modelFactory(&modelFac),
    m_rendererFactory(&rendererFac),
    m_controllerFactory(&controllerFac)
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
