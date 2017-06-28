#include "projectiles/Projectile.h"

#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/controllers/ProjectileController.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Configuration.h"

#include <memory>

Projectile::Projectile(
    int id,
    const ComponentFactory<Projectile, ProjectileModel>& modelFac,
    const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac,
    const ComponentFactory<Projectile, ProjectileController>& controllerFac) :
    m_model(modelFac.create(*this, modelFac.createCommonDataStorage())),
    m_renderer(rendererFac.create(*this, rendererFac.createCommonDataStorage())),
    m_controller(controllerFac.create(*this, controllerFac.createCommonDataStorage())),
    m_id(id)
{
}
Projectile::Projectile(const Projectile& other) :
    m_model(other.m_model->clone(*this)),
    m_renderer(other.m_renderer->clone(*this)),
    m_controller(other.m_controller->clone(*this)),
    m_id(other.m_id)
{
}
Projectile::Projectile(Projectile&& other) :
    m_model(std::move(other.m_model)),
    m_renderer(std::move(other.m_renderer)),
    m_controller(std::move(other.m_controller)),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);
}
Projectile& Projectile::operator=(const Projectile& other)
{
    m_model = other.m_model->clone(*this);
    m_renderer = other.m_renderer->clone(*this);
    m_controller = other.m_controller->clone(*this);
    m_id = other.m_id;

    return *this;
}
Projectile& Projectile::operator=(Projectile&& other)
{
    m_model = std::move(other.m_model);
    m_renderer = std::move(other.m_renderer);
    m_controller = std::move(other.m_controller);
    m_id = other.m_id;

    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);

    return *this;
}
Projectile::~Projectile()
{

}

void Projectile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);
}

void Projectile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    m_renderer->draw(renderTarget, renderStates);
}
void Projectile::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    m_renderer->drawMeta(renderTarget, renderStates);
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
const ProjectileController& Projectile::controller() const
{
    return *m_controller;
}
ProjectileController& Projectile::controller()
{
    return *m_controller;
}

int Projectile::id() const
{
    return m_id;
}
void Projectile::onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    m_model->onProjectileInstantiated(world, parentEntity, hintedPosition);
    m_renderer->onProjectileInstantiated(world, parentEntity, hintedPosition);
    m_controller->onProjectileInstantiated(world, parentEntity, hintedPosition);
}
void Projectile::onProjectileCloned()
{
    m_model->onProjectileCloned();
    m_renderer->onProjectileCloned();
    m_controller->onProjectileCloned();
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
