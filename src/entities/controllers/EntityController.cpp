#include "entities/controllers/EntityController.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "Logger.h"

EntityController::EntityController(Entity& owner) :
    EntityComponent(owner)
{

}
EntityController::EntityController(const EntityController& other, Entity& owner) :
    EntityComponent(other, owner)
{

}
EntityController::~EntityController()
{

}

void EntityController::loadFromConfiguration(ConfigurationNode& config)
{

}

void EntityController::update(World& world, float dt)
{

}

void EntityController::accelerate(const ls::Vec2F& dv)
{

}

void EntityController::dealDamage(int damage)
{
    const int currentHealth = m_owner->model().health();
    m_owner->model().setHealth(currentHealth - damage);
    
    Logger::instance().log(Logger::Priority::Debug, std::to_string(damage) + " damage dealt");
}

std::unique_ptr<EntityController> EntityController::clone(Entity& owner) const
{
    return std::make_unique<EntityController>(*this, owner);
}
