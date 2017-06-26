#include "entities/controllers/EntityController.h"

#include "ResourceLoaders.h"

REGISTER_ENTITY_CONTROLLER_TYPE(EntityController)

EntityController::EntityController(Entity& owner) :
    EntityComponent(owner)
{

}
EntityController::EntityController(Entity& owner, ComponentCommonData* commonData) :
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

void EntityController::move(const ls::Vec2F& factor, float dt)
{

}
void EntityController::accelerate(const ls::Vec2F& dv)
{

}

std::unique_ptr<EntityController> EntityController::clone(Entity& owner) const
{
    return std::make_unique<EntityController>(*this, owner);
}
