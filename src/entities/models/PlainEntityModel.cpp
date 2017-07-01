#include "entities/models/PlainEntityModel.h"

using namespace ls;

REGISTER_ENTITY_MODEL_TYPE(PlainEntityModel)

PlainEntityModel::PlainEntityModel(Entity& owner, ComponentCommonData* commonData) :
    EntityModel(owner),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f),
    m_health(1),
    m_maxHealth(1),
    m_group(AggroGroupId::Unfriendly),
    m_corpseTile(nullptr)
{

}
PlainEntityModel::PlainEntityModel(const PlainEntityModel& other, Entity& owner) :
    EntityModel(other, owner),
    m_directionOfMove(other.m_directionOfMove),
    m_distanceTravelled(other.m_distanceTravelled),
    m_health(other.m_health),
    m_maxHealth(other.m_maxHealth),
    m_group(other.m_group),
    m_corpseTile(other.m_corpseTile),
    m_lootRandomizer(other.m_lootRandomizer)
{

}
PlainEntityModel::~PlainEntityModel()
{

}

void PlainEntityModel::loadFromConfiguration(ConfigurationNode& config)
{
    m_health = m_maxHealth = config["maxHealth"].get<int>();
    m_corpseTile = ResourceManager::instance().get<TilePrefab>(config["corpseTile"].get<std::string>());
    m_lootRandomizer.loadFromConfiguration(config["lootRandomizationGuidelines"]);
    m_group = AggroGroupIdHelper::stringToEnum(config["group"].get<std::string>());
}

EntityCollider PlainEntityModel::collider()
{
    return EntityCollider(*m_owner, ls::Circle2F(m_position, 6.0f));
}

const Vec2F& PlainEntityModel::position() const
{
    return m_position;
}
void PlainEntityModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlainEntityModel::velocity() const
{
    return m_velocity;
}
void PlainEntityModel::setVelocity(const ls::Vec2F& newVelocity)
{
    m_velocity = newVelocity;
}

float PlainEntityModel::distanceTravelled() const
{
    return m_distanceTravelled;
}
void PlainEntityModel::setDistanceTravelled(float newDistanceTravelled)
{
    m_distanceTravelled = newDistanceTravelled;
}
int PlainEntityModel::health() const
{
    return m_health;
}
int PlainEntityModel::maxHealth() const
{
    return m_maxHealth;
}
void PlainEntityModel::setHealth(int newHealth)
{
    m_health = newHealth;
}
TileStack PlainEntityModel::createCorpse() const
{
    TileStack corpse = TileStack(m_corpseTile->instantiate(), 1);
    m_lootRandomizer.randomize(*(corpse.tile().model().inventory()));
    return corpse;
}
AggroGroupId PlainEntityModel::group() const
{
    return m_group;
}

float PlainEntityModel::maxSpeed() const
{
    return 64.0f;
}

EntityModel::Direction PlainEntityModel::directionOfMove() const
{
    return m_directionOfMove;
}
void PlainEntityModel::setDirectionOfMovement(EntityModel::Direction newDirection)
{
    m_directionOfMove = newDirection;
}
std::unique_ptr<EntityModel> PlainEntityModel::clone(Entity& owner) const
{
    return std::make_unique<PlainEntityModel>(*this, owner);
}
