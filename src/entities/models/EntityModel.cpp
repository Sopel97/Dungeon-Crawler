#include "EntityModel.h"

#include "../LibS/make_unique.h"

using namespace Geo;

const Geo::Vec2F EntityModel::m_zeroVector{0.0f, 0.0f};

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
const Geo::Vec2F& EntityModel::position() const
{
    return m_zeroVector;
}
void EntityModel::setPosition(const Vec2F& newPosition)
{

}
const Geo::Vec2F& EntityModel::velocity() const
{
    return m_zeroVector;
}
std::unique_ptr<EntityModel> EntityModel::clone() const
{
    return std::make_unique<EntityModel>(*this);
}
std::unique_ptr<EntityModel> EntityModel::create(Entity* owner) const
{
    return std::make_unique<EntityModel>(owner);
}
