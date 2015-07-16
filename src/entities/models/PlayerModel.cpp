#include "PlayerModel.h"

using namespace Geo;

PlayerModel::PlayerModel(Entity* owner, Player* player) :
    EntityModel(owner),
    m_playerOwner(player),
    m_directionOfMove(Direction::South)
{

}
PlayerModel::PlayerModel(const PlayerModel& other) :
    EntityModel(other.m_owner),
    m_playerOwner(other.m_playerOwner),
    m_directionOfMove(other.m_directionOfMove)
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
    return 8.0f;
}

const Vec2F& PlayerModel::position() const
{
    return m_position;
}
void PlayerModel::setPosition(const Geo::Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlayerModel::velocity() const
{
    return m_velocity;
}
void PlayerModel::setVelocity(const Geo::Vec2F& newVelocity)
{
    m_velocity = newVelocity;
}

float PlayerModel::minSpeed() const
{
    return 10.0f;
}
float PlayerModel::maxSpeed() const
{
    return 64.0f;
}

EntityModel::Direction PlayerModel::directionOfMove() const
{
    return m_directionOfMove;
}
void PlayerModel::setDirectionOfMove(EntityModel::Direction newDirection)
{
    m_directionOfMove = newDirection;
}
std::unique_ptr<EntityModel> PlayerModel::clone() const
{
    return std::make_unique<PlayerModel>(*this);
}
std::unique_ptr<EntityModel> PlayerModel::create(Entity* owner) const
{
    return std::make_unique<PlayerModel>(owner, nullptr);
}
