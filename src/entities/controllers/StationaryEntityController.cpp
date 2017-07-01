#include "entities/controllers/StationaryEntityController.h"

#include "World.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

REGISTER_ENTITY_CONTROLLER_TYPE(StationaryEntityController)

StationaryEntityController::StationaryEntityController(Entity& owner, ComponentCommonData* commonData) :
    EntityController(owner),
    m_velocity(0,0)
{

}

StationaryEntityController::StationaryEntityController(const StationaryEntityController& other, Entity& owner) :
    EntityController(other, owner),
    m_velocity(other.m_velocity)
{

}
StationaryEntityController::~StationaryEntityController()
{

}

void StationaryEntityController::loadFromConfiguration(ConfigurationNode& config)
{

}

void StationaryEntityController::update(World& world, float dt)
{
    //m_owner->model().setHealth(m_owner->model().health() - 1);

    m_owner->model().setPosition(m_owner->model().position() + m_velocity * dt);
}

void StationaryEntityController::accelerate(const ls::Vec2F& dv)
{
    m_velocity += dv;
}

std::unique_ptr<EntityController> StationaryEntityController::clone(Entity& owner) const
{
    return std::make_unique<StationaryEntityController>(*this, owner);
}
