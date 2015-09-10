#include "Player.h"

#include "Entity.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"

#include "World.h"

#include "EventDispatcher.h"



#include "../LibS/Geometry.h"

#include <algorithm>

using namespace ls;

Player::Player(Game& game) :
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

void Player::processAsyncKeyboardInput(World& world, float dt) //TODO: make it update the player entity so it moves in the next update. Don't interact with world in this function.
{
    if(m_playerEntity == nullptr) return;

    constexpr float acc = 200.0f;
    float drag = world.drag(m_playerEntity->model().position());
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_playerEntity->controller().accelerate(Vec2F(-acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_playerEntity->controller().accelerate(Vec2F(acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_playerEntity->controller().accelerate(Vec2F(0.0f, -acc * dt * drag));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_playerEntity->controller().accelerate(Vec2F(0.0f, acc * dt * drag));
}