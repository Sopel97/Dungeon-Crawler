#include "entities/models/EntityModel.h"

using namespace ls;

const ls::Vec2F EntityModel::m_someVector {0.0f, 0.0f};

EntityModel::EntityModel(Entity& owner) :
    EntityComponent(owner)
{

}
EntityModel::EntityModel(const EntityModel& other, Entity& owner) :
    EntityComponent(other, owner)
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
EntityCollider EntityModel::collider()
{
    return EntityCollider(*m_owner, ls::Circle2F());
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
int EntityModel::health() const
{
    return 1;
}
int EntityModel::maxHealth() const
{
    return 1;
}
void EntityModel::setHealth(int newHealth)
{

}
TileStack EntityModel::createCorpse() const
{
    return TileStack();
}
AggroGroupId EntityModel::group() const
{
    return AggroGroupId::Neutral;
}
TileStack& EntityModel::ammo()
{
    static TileStack a;
    return a;
}

Vec2F EntityModel::displacementWhenMoved(float dt)
{
    return Vec2F(0.0f, 0.0f);
}

void EntityModel::onEntityInstantiated(const ls::Vec2F& pos)
{
    this->setPosition(pos);
}

float EntityModel::maxSpeed() const
{
    return 10.0f;
}

EntityModel::Direction EntityModel::directionOfMove() const
{
    return Direction::South;
}
void EntityModel::setDirectionOfMovement(EntityModel::Direction newDirection)
{

}
std::unique_ptr<EntityModel> EntityModel::clone(Entity& owner) const
{
    return std::make_unique<EntityModel>(*this, owner);
}
