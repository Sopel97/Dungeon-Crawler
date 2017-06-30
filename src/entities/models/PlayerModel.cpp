#include "entities/models/PlayerModel.h"

#include "Player.h"

using namespace ls;

REGISTER_ENTITY_MODEL_TYPE(PlayerModel)

PlayerModel::PlayerModel(Entity& owner, ComponentCommonData* commonData) :
    EntityModel(owner),
    m_playerOwner(),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f)
{

}
PlayerModel::PlayerModel(Player& player, Entity& owner) :
    EntityModel(owner),
    m_playerOwner(&player),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f)
{

}
PlayerModel::PlayerModel(const PlayerModel& other, Entity& owner) :
    EntityModel(other, owner),
    m_playerOwner(other.m_playerOwner),
    m_directionOfMove(other.m_directionOfMove),
    m_distanceTravelled(other.m_distanceTravelled)
{

}
PlayerModel::~PlayerModel()
{

}

void PlayerModel::loadFromConfiguration(ConfigurationNode& config)
{

}

bool PlayerModel::hasCollider() const
{
    return true;
}
float PlayerModel::colliderRadius() const
{
    return 6.0f;
}

const Vec2F& PlayerModel::position() const
{
    return m_position;
}
void PlayerModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlayerModel::velocity() const
{
    return m_velocity;
}
void PlayerModel::setVelocity(const ls::Vec2F& newVelocity)
{
    m_velocity = newVelocity;
}

float PlayerModel::distanceTravelled() const
{
    return m_distanceTravelled;
}
void PlayerModel::setDistanceTravelled(float newDistanceTravelled)
{
    m_distanceTravelled = newDistanceTravelled;
}
AggroGroupId PlayerModel::group() const
{
    return AggroGroupId::Friendly;
}
TileStack& PlayerModel::ammo()
{
    return m_playerOwner->ammo();
}

Vec2F PlayerModel::displacementWhenMoved(float dt)
{
    return m_velocity * dt;
}

float PlayerModel::maxSpeed() const
{
    return 64.0f;
}

EntityModel::Direction PlayerModel::directionOfMove() const
{
    return m_directionOfMove;
}
void PlayerModel::setDirectionOfMovement(EntityModel::Direction newDirection)
{
    m_directionOfMove = newDirection;
}
std::unique_ptr<EntityModel> PlayerModel::clone(Entity& owner) const
{
    return std::make_unique<PlayerModel>(*this, owner);
}
