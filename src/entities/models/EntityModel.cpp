#include "EntityModel.h"

#include "../LibS/make_unique.h"

using namespace ls;

const ls::Vec2F EntityModel::m_someVector {0.0f, 0.0f};

EntityModel::EntityModel(Entity* owner) :
    m_owner(owner)
{

}
EntityModel::EntityModel(const EntityModel& other) :
    m_owner(other.m_owner)
{

}
EntityModel::~EntityModel()
{

}

void EntityModel::loadFromConfiguration(ConfigurationNode& config)
{

}

const Entity* EntityModel::owner() const
{
    return m_owner;
}

void EntityModel::setOwner(Entity* newOwner)
{
    m_owner = newOwner;
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
    return 0.0f;
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
std::unique_ptr<EntityModel> EntityModel::create(Entity* owner) const
{
    return std::make_unique<EntityModel>(owner);
}
