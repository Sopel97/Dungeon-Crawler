#include "Entity.h"

#include "EntityModel.h"
#include "EntityView.h"
#include "EntityController.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Configuration.h"



#include <memory>

int Entity::m_lastId = -1;

Entity::Entity(std::unique_ptr<EntityModel>&& model, std::unique_ptr<EntityView>&& view, std::unique_ptr<EntityController>&& controller) :
    m_model(std::move(model)),
    m_view(std::move(view)),
    m_controller(std::move(controller)),
    m_id(++m_lastId)
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Entity::Entity(const Entity& other) :
    m_model(other.m_model->clone()),
    m_view(other.m_view->clone()),
    m_controller(other.m_controller->clone()),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Entity::~Entity()
{

}

void Entity::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_view->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);
}

void Entity::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    m_view->draw(renderTarget, renderStates);
}

const EntityModel& Entity::model() const
{
    return *m_model;
}
EntityModel& Entity::model()
{
    return *m_model;
}
const EntityView& Entity::view() const
{
    return *m_view;
}
EntityView& Entity::view()
{
    return *m_view;
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

std::unique_ptr<Entity> Entity::clone() const
{
    return std::make_unique<Entity>(*this);
}
