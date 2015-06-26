#include "EntityController.h"

#include "../LibS/make_unique.h"

EntityController::EntityController(Entity* owner) :
    m_owner(owner)
{

}
EntityController::EntityController(const EntityController& other) :
    m_owner(other.m_owner)
{

}
EntityController::~EntityController()
{

}

void EntityController::loadFromConfiguration(ConfigurationNode& config)
{

}

const Entity* EntityController::owner() const
{
    return m_owner;
}

void EntityController::setOwner(Entity* newOwner)
{
    m_owner = newOwner;
}

void EntityController::update(float dt)
{

}

std::unique_ptr<EntityController> EntityController::clone() const
{
    return std::make_unique<EntityController>(*this);
}
std::unique_ptr<EntityController> EntityController::create(Entity* owner) const
{
    return std::make_unique<EntityController>(owner);
}
