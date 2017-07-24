#include "entities/Entity.h"

#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Configuration.h"

#include <memory>

Entity::Entity(
    int id,
    const ComponentFactory<Entity, EntityModel>& modelFac,
    const ComponentFactory<Entity, EntityRenderer>& rendererFac) :
    m_model(modelFac.createWithNewCommonData(*this)),
    m_renderer(rendererFac.createWithNewCommonData(*this)),
    m_id(id)
{
}
Entity::Entity(
    int id,
    std::unique_ptr<EntityModel> model,
    std::unique_ptr<EntityRenderer> renderer) :
    m_model(std::move(model)),
    m_renderer(std::move(renderer)),
    m_id(id)
{
}
Entity::Entity(const Entity& other) :
    m_model(other.m_model->clone(*this)),
    m_renderer(other.m_renderer->clone(*this)),
    m_id(other.m_id)
{
}
Entity::Entity(Entity&& other) :
    m_model(std::move(other.m_model)),
    m_renderer(std::move(other.m_renderer)),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
}
Entity& Entity::operator=(const Entity& other)
{
    m_model = other.m_model->clone(*this);
    m_renderer = other.m_renderer->clone(*this);
    m_id = other.m_id;

    return *this;
}
Entity& Entity::operator=(Entity&& other)
{
    m_model = std::move(other.m_model);
    m_renderer = std::move(other.m_renderer);
    m_id = other.m_id;

    m_model->setOwner(this);
    m_renderer->setOwner(this);

    return *this;
}
Entity::~Entity()
{

}

void Entity::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
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

int Entity::id() const
{
    return m_id;
}
void Entity::onEntityInstantiated(const ls::Vec2F& pos)
{
    m_model->onEntityInstantiated(pos);
    m_renderer->onEntityInstantiated(pos);
}
void Entity::onEntityCloned()
{
    m_model->onEntityCloned();
    m_renderer->onEntityCloned();
}

std::unique_ptr<Entity> Entity::clone() const
{
    std::unique_ptr<Entity> entityClone = std::make_unique<Entity>(*this);

    entityClone->onEntityCloned();

    return entityClone;
}
std::unique_ptr<Entity> Entity::instantiate(const ls::Vec2F& pos) const
{
    std::unique_ptr<Entity> entityClone = std::make_unique<Entity>(*this);

    entityClone->onEntityInstantiated(pos);

    return entityClone;
}
