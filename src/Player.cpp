#include "Player.h"

#include "entities/models/PlayerModel.h"
#include "entities/renderers/PlayerRenderer.h"

#include "InventorySlotLocation.h"

#include "window/Scene.h"

#include "World.h"

#include "EventDispatcher.h"

#include "../LibS/Geometry.h"

#include <algorithm>

using namespace ls;

Player::Player(WindowSpaceManager& wsm, Game& game, TileTransferMediator& tileTransferMediator) :
    m_wsm(&wsm),
    m_playerEntity(-1, std::make_unique<PlayerModel>(*this, m_playerEntity), std::make_unique<PlayerRenderer>(*this, m_playerEntity)),
    m_playerUi(wsm, *this),
    m_inventorySystem(wsm, *this, tileTransferMediator),
    m_equipmentInventory()
{
    m_inventorySystem.openPermanentInventory(m_equipmentInventory, "");
}

bool Player::tryInteractWithExternalInventory(Tile& tile, Inventory& inventory, const TileLocation& location)
{
    return m_inventorySystem.tryInteractWithExternalInventory(tile, inventory, location);
}
bool Player::tryInteractWithInternalInventory(Tile& tile, Inventory& inventory, const InventorySlotLocation& slot)
{
    return m_inventorySystem.tryInteractWithInternalInventory(tile, inventory, slot);
}

void Player::processAsyncKeyboardInput(World& world, float dt) //TODO: make it update the player entity so it moves in the next update. Don't interact with world in this function.
{
    constexpr float acc = 200.0f / 32.0f;
    float drag = world.drag(m_playerEntity.model().position());
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_playerEntity.model().accelerate(Vec2F(-acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_playerEntity.model().accelerate(Vec2F(acc * dt * drag, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_playerEntity.model().accelerate(Vec2F(0.0f, -acc * dt * drag));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_playerEntity.model().accelerate(Vec2F(0.0f, acc * dt * drag));
}

PlayerUi& Player::playerUi()
{
    return m_playerUi;
}
const PlayerUi& Player::playerUi() const
{
    return m_playerUi;
}
Entity& Player::entity()
{
    return m_playerEntity;
}
const Entity& Player::entity() const
{
    return m_playerEntity;
}
InventorySystem& Player::inventorySystem()
{
    return m_inventorySystem;
}
const InventorySystem& Player::inventorySystem() const
{
    return m_inventorySystem;
}
TileStack& Player::ammo()
{
    return m_equipmentInventory.at(PlayerEquipmentInventory::SlotType::Ammo);
}
TileStack& Player::weapon()
{
    return m_equipmentInventory.at(PlayerEquipmentInventory::SlotType::PrimaryWeapon);
}
const PlayerEquipmentInventory& Player::equipmentInventory() const
{
    return m_equipmentInventory;
}
void Player::showTileDescription(const TileDescription& description)
{
    m_playerUi.showTileDescription(description);
}
void Player::showTileDescription(TileDescription&& description)
{
    m_playerUi.showTileDescription(std::move(description));
}

void Player::attack(World& world, const ls::Vec2F& pos)
{
    TileStack& weapon = this->weapon();
    if (weapon.isEmpty()) return;
    
    auto attackResult = weapon.attack(world, *this, pos);

    const int ammoSlot = m_equipmentInventory.slotId(PlayerEquipmentInventory::SlotType::Ammo);
    const int weaponSlot = m_equipmentInventory.slotId(PlayerEquipmentInventory::SlotType::PrimaryWeapon);
    if(attackResult.ammoUsed > 0) m_equipmentInventory.removeTiles(ammoSlot, attackResult.ammoUsed);
    if(attackResult.weaponUsed > 0) m_equipmentInventory.removeTiles(weaponSlot, attackResult.weaponUsed);
}