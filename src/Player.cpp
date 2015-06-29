#include "Player.h"

#include "Entity.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"

#include "../LibS/make_unique.h"

Player::Player() :
    m_playerEntity(nullptr)
{

}

Entity* Player::createPlayerEntity()
{
    return m_playerEntity = new Entity(std::make_unique<PlayerModel>(nullptr, this), std::make_unique<PlayerView>(nullptr, this), std::make_unique<PlayerController>(nullptr, this));
}
