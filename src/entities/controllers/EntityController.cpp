#include "entities/controllers/EntityController.h"

EntityController::EntityController() :
    EntityComponent()
{

}
EntityController::EntityController(const EntityController& other) :
    EntityComponent(other)
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

std::unique_ptr<EntityController> EntityController::clone() const
{
    return std::make_unique<EntityController>(*this);
}
