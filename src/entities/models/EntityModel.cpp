#include "entities/models/EntityModel.h"

#include "AttributeArray.h"

#include "Logger.h"

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
const AttributeArray& EntityModel::attributes() const
{
    static AttributeArray a;
    return a;
}
std::optional<Light> EntityModel::light() const
{
    return std::nullopt;
}
bool EntityModel::addEffect(const Effect& effect)
{
    return false;
}
bool EntityModel::hasEffect(int id) const
{
    return false;
}
bool EntityModel::removeEffect(int id)
{
    return false;
}

void EntityModel::onEntityInstantiated(const ls::Vec2F& pos)
{
    this->setPosition(pos);
}

EntityModel::Direction EntityModel::directionOfMove() const
{
    return Direction::South;
}

void EntityModel::update(World& world, float dt)
{

}

void EntityModel::accelerate(const ls::Vec2F& dv)
{

}

void EntityModel::dealDamage(int damage)
{
    const int currentHealth = health();
    setHealth(currentHealth - damage);

    Logger::instance().log(Logger::Priority::Debug, std::to_string(damage) + " damage dealt");
}
std::unique_ptr<EntityModel> EntityModel::clone(Entity& owner) const
{
    return std::make_unique<EntityModel>(*this, owner);
}
