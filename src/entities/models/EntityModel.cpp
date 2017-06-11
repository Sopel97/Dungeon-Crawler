#include "entities/models/EntityModel.h"

using namespace ls;

const ls::Vec2F EntityModel::m_someVector {0.0f, 0.0f};

EntityModel::EntityModel() :
    EntityComponent()
{

}
EntityModel::EntityModel(const EntityModel& other) :
    EntityComponent(other)
{

}
EntityModel::~EntityModel()
{

}

void EntityModel::loadFromConfiguration(ConfigurationNode& config)
{

}

bool EntityModel::hasCollider() const
{
    return false;
}
float EntityModel::colliderRadius() const
{
    return 0.0f;
}
const ls::Vec2F& EntityModel::position() const
{
    return m_someVector;
}
void EntityModel::setPosition(const ls::Vec2F& newPosition)
{

}
const ls::Vec2F& EntityModel::velocity() const
{
    return m_someVector;
}
void EntityModel::setVelocity(const ls::Vec2F& newVelocity)
{

}

float EntityModel::distanceTravelled() const
{
    return 0.0f;
}
void EntityModel::setDistanceTravelled(float newDistanceTravelled)
{

}

Vec2F EntityModel::displacementWhenMoved(float dt)
{
    return Vec2F(0.0f, 0.0f);
}

float EntityModel::maxSpeed() const
{
    return 10.0f;
}

EntityModel::Direction EntityModel::directionOfMove() const
{
    return Direction::South;
}
void EntityModel::setDirectionOfMove(EntityModel::Direction newDirection)
{

}
std::unique_ptr<EntityModel> EntityModel::clone() const
{
    return std::make_unique<EntityModel>(*this);
}
