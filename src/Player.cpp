#include "Player.h"

#include "entities/Entity.h"
#include "entities/models/PlayerModel.h"
#include "entities/renderers/PlayerRenderer.h"
#include "entities/controllers/PlayerController.h"


#include "World.h"

#include "EventDispatcher.h"

#include "../LibS/Geometry.h"

#include <algorithm>

using namespace ls;

Player::Player(Game& game, const WindowSpaceManager::WindowRegionFullLocalization& loc) :
    m_playerEntity(nullptr),
    m_playerUi(*this, loc),
    m_inventorySystem(*this)
{

}

Entity* Player::createPlayerEntity()
{
    return m_playerEntity = new Entity(std::make_unique<PlayerModel>(nullptr, this), std::make_unique<PlayerRenderer>(nullptr, this), std::make_unique<PlayerController>(nullptr, this));
}

void Player::onKeyPressed(sf::Event::KeyEvent& keyEvent)
{

}

bool Player::tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location)
{
    return m_inventorySystem.tryInteractWithExternalInventory(inventory, location);
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

PlayerUi& Player::playerUi()
{
    return m_playerUi;
}
const PlayerUi& Player::playerUi() const
{
    return m_playerUi;
}
InventorySystem& Player::inventorySystem()
{
    return m_inventorySystem;
}
const InventorySystem& Player::inventorySystem() const
{
    return m_inventorySystem;
}