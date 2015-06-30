#include "Player.h"

#include "Entity.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"

#include "../LibS/make_unique.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

Player::Player() :
    m_playerEntity(nullptr)
{

}

Entity* Player::createPlayerEntity()
{
    return m_playerEntity = new Entity(std::make_unique<PlayerModel>(nullptr, this), std::make_unique<PlayerView>(nullptr, this), std::make_unique<PlayerController>(nullptr, this));
}

void Player::onKeyPressed(sf::Event::KeyEvent& keyEvent)
{

}

void Player::processAsyncKeyboardInput(float dt)
{
    constexpr float acc = 100.0f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_playerEntity->controller().accelerate(Vec2F(-acc*dt, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_playerEntity->controller().accelerate(Vec2F(acc*dt, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_playerEntity->controller().accelerate(Vec2F(0.0f, -acc*dt));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_playerEntity->controller().accelerate(Vec2F(0.0f, acc*dt));
}
