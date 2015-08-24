#include "Player.h"

#include "Entity.h"
#include "PlayerModel.h"
#include "PlayerView.h"
#include "PlayerController.h"

#include "World.h"

#include "Inventory.h"

#include "PlayerUi.h"

#include "AttemptToInteractWithExternalInventory.h"

#include "EventDispatcher.h"

#include "../LibS/make_unique.h"

#include "../LibS/Geometry.h"

#include <algorithm>

using namespace ls;

Player::Player() :
    m_playerEntity(nullptr)
{
    EventDispatcher::instance().subscribe<AttemptToInteractWithExternalInventory>(std::bind(&Player::onAttemptToInteractWithExternalInventory, this, std::placeholders::_1));
}

Entity* Player::createPlayerEntity()
{
    return m_playerEntity = new Entity(std::make_unique<PlayerModel>(nullptr, this), std::make_unique<PlayerView>(nullptr, this), std::make_unique<PlayerController>(nullptr, this));
}
PlayerUi* Player::createPlayerUi()
{
    return m_playerUi = new PlayerUi(*this);
}

void Player::onKeyPressed(sf::Event::KeyEvent& keyEvent)
{

}

void Player::processAsyncKeyboardInput(World* world, float dt)
{
    constexpr float acc = 200.0f;
    float drag = world->drag(m_playerEntity->model().position());
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_playerEntity->controller().accelerate(Vec2F(-acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_playerEntity->controller().accelerate(Vec2F(acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_playerEntity->controller().accelerate(Vec2F(0.0f, -acc * dt * drag));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_playerEntity->controller().accelerate(Vec2F(0.0f, acc * dt * drag));
}
bool Player::tryOpenInventory(Inventory* inventory, int x, int y)
{
    if(m_playerUi->tryOpenInventory(inventory))
    {
        m_externalInventories.push_back(ExternalInventory{inventory, x, y});
        return true;
    }
    return false;
}
void Player::closeInventory(Inventory* inventory)
{
    m_externalInventories.erase(std::remove(m_externalInventories.begin(), m_externalInventories.end(), inventory), m_externalInventories.end());
    m_playerUi->closeInventory(inventory);
}
bool Player::isInventoryOpened(Inventory* inventory)
{
    return std::find(m_externalInventories.begin(), m_externalInventories.end(), inventory) != m_externalInventories.end();
}

void Player::onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event)
{
    tryOpenInventory(event.inventory(), event.x(), event.y());
}

PlayerEquipmentInventory& Player::equipmentInventory()
{
    return m_equipmentInventory;
}
