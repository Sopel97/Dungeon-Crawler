#include "PlayerModel.h"

using namespace Geo;

PlayerModel::PlayerModel(Entity* owner, Player* player) :
    EntityModel(owner),
    m_playerOwner(player)
{

}
PlayerModel::PlayerModel(const PlayerModel& other) :
    EntityModel(other.m_owner),
    m_playerOwner(other.m_playerOwner)
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
void PlayerModel::setPosition(const Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlayerModel::velocity() const
{
    return m_velocity;
}

std::unique_ptr<EntityModel> PlayerModel::clone() const
{
    return std::make_unique<PlayerModel>(*this);
}
std::unique_ptr<EntityModel> PlayerModel::create(Entity* owner) const
{
    return std::make_unique<PlayerModel>(owner, nullptr);
}
