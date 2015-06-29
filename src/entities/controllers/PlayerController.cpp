#include "PlayerController.h"

#include "World.h"

#include "../LibS/make_unique.h"

PlayerController::PlayerController(Entity* owner, Player* player) :
    EntityController(owner),
    m_playerOwner(player)
{

}
PlayerController::PlayerController(const PlayerController& other) :
    EntityController(other.m_owner),
    m_playerOwner(other.m_playerOwner)
{

}
PlayerController::~PlayerController()
{

}

void PlayerController::loadFromConfiguration(ConfigurationNode& config)
{

}

void PlayerController::update(World* world, float dt)
{

}

std::unique_ptr<EntityController> PlayerController::clone() const
{
    return std::make_unique<PlayerController>(*this);
}
std::unique_ptr<EntityController> PlayerController::create(Entity* owner) const
{
    return std::make_unique<PlayerController>(owner, nullptr);
}
