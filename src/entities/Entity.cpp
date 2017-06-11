#include "entities/Entity.h"

#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/controllers/EntityController.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Configuration.h"

#include <memory>

Entity::Entity(int id, std::unique_ptr<EntityModel>&& model, std::unique_ptr<EntityRenderer>&& renderer, std::unique_ptr<EntityController>&& controller) :
    m_model(std::move(model)),
    m_renderer(std::move(renderer)),
    m_controller(std::move(controller)),
    m_id(id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);
}
Entity::Entity(const Entity& other) :
    m_model(other.m_model->clone()),
    m_renderer(other.m_renderer->clone()),
    m_controller(other.m_controller->clone()),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);
}
Entity::~Entity()
{

}

void Entity::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);
}

void Entity::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    m_renderer->draw(renderTarget, renderStates);
}
void Entity::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    m_renderer->drawMeta(renderTarget, renderStates);
}

const EntityModel& Entity::model() const
{
    return *m_model;
}
EntityModel& Entity::model()
{
    return *m_model;
}
const EntityRenderer& Entity::renderer() const
{
    return *m_renderer;
}
EntityRenderer& Entity::renderer()
{
    return *m_renderer;
}
const EntityController& Entity::controller() const
{
    return *m_controller;
}
EntityController& Entity::controller()
{
    return *m_controller;
}

int Entity::id() const
{
    return m_id;
}
void Entity::onEntityInstantiated()
{
    m_model->onEntityInstantiated();
    m_renderer->onEntityInstantiated();
    m_controller->onEntityInstantiated();
}
void Entity::onEntityCloned()
{
    m_model->onEntityCloned();
    m_renderer->onEntityCloned();
    m_controller->onEntityCloned();
}

std::unique_ptr<Entity> Entity::clone() const
{
    std::unique_ptr<Entity> entityClone = std::make_unique<Entity>(*this);

    entityClone->onEntityCloned();

    return entityClone;
}
std::unique_ptr<Entity> Entity::instantiate() const
{
    std::unique_ptr<Entity> entityClone = std::make_unique<Entity>(*this);

    entityClone->onEntityInstantiated();

    return entityClone;
}
