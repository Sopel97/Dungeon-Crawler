#include "projectiles/Projectile.h"

#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "configuration/Configuration.h"

#include <memory>

Projectile::Projectile(
    int id,
    const ComponentFactory<Projectile, ProjectileModel>& modelFac,
    const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac) :
    m_model(modelFac.createWithNewCommonData(*this)),
    m_renderer(rendererFac.createWithNewCommonData(*this)),
    m_id(id)
{
}
Projectile::Projectile(const Projectile& other) :
    m_model(other.m_model->clone(*this)),
    m_renderer(other.m_renderer->clone(*this)),
    m_id(other.m_id)
{
}
Projectile::Projectile(Projectile&& other) :
    m_model(std::move(other.m_model)),
    m_renderer(std::move(other.m_renderer)),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
}
Projectile& Projectile::operator=(const Projectile& other)
{
    m_model = other.m_model->clone(*this);
    m_renderer = other.m_renderer->clone(*this);
    m_id = other.m_id;

    return *this;
}
Projectile& Projectile::operator=(Projectile&& other)
{
    m_model = std::move(other.m_model);
    m_renderer = std::move(other.m_renderer);
    m_id = other.m_id;

    m_model->setOwner(this);
    m_renderer->setOwner(this);

    return *this;
}
Projectile::~Projectile()
{

}

void Projectile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
}

const ProjectileModel& Projectile::model() const
{
    return *m_model;
}
ProjectileModel& Projectile::model()
{
    return *m_model;
}
const ProjectileRenderer& Projectile::renderer() const
{
    return *m_renderer;
}
ProjectileRenderer& Projectile::renderer()
{
    return *m_renderer;
}

int Projectile::id() const
{
    return m_id;
}
void Projectile::onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    m_model->onProjectileInstantiated(world, parentEntity, hintedPosition);
    m_renderer->onProjectileInstantiated(world, parentEntity, hintedPosition);
}
void Projectile::onProjectileCloned()
{
    m_model->onProjectileCloned();
    m_renderer->onProjectileCloned();
}
void Projectile::onCollidedWithEntity(EntityCollider& entityCollider)
{
    m_model->onCollidedWithEntity(entityCollider);
    m_renderer->onCollidedWithEntity(entityCollider);
}
void Projectile::onCollidedWithTile(TileCollider& tileCollider)
{
    m_model->onCollidedWithTile(tileCollider);
    m_renderer->onCollidedWithTile(tileCollider);
}
void Projectile::onParentEntityDeleted()
{
    m_model->onParentEntityDeleted();
    m_renderer->onParentEntityDeleted();
}

std::unique_ptr<Projectile> Projectile::clone() const
{
    std::unique_ptr<Projectile> projectileClone = std::make_unique<Projectile>(*this);

    projectileClone->onProjectileCloned();

    return projectileClone;
}
std::unique_ptr<Projectile> Projectile::instantiate(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) const
{
    std::unique_ptr<Projectile> projectileClone = std::make_unique<Projectile>(*this);

    projectileClone->onProjectileInstantiated(world, parentEntity, hintedPosition);

    return projectileClone;
}
