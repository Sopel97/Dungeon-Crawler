#include "Player.h"

#include "entities/models/PlayerModel.h"
#include "entities/renderers/PlayerRenderer.h"

#include "InventorySlotLocation.h"

#include "window/Scene.h"

#include "World.h"

#include "EventDispatcher.h"

#include "AttributeSet.h"

#include "OscillatingLightSource.h"

#include "GameTime.h"

#include "../LibS/Geometry.h"

#include <algorithm>

using namespace ls;

Player::Player(WindowSpaceManager& wsm, Game& game, TileTransferMediator& tileTransferMediator) :
    m_wsm(&wsm),
    m_world(nullptr),
    m_playerEntity(-1, std::make_unique<PlayerModel>(*this, m_playerEntity), std::make_unique<PlayerRenderer>(*this, m_playerEntity)),
    m_playerUi(wsm, *this),
    m_inventorySystem(wsm, *this, tileTransferMediator),
    m_equipmentInventory(),
    m_weaponUseTimeLeft(0.0f),
    m_weaponCooldownLeft(0.0f),
    m_forcedDirection(std::nullopt)
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
void Player::update(float dt)
{
    updateAttributes();
    updateEquipedTiles();

    if (m_weaponUseTimeLeft <= 0.0f)
    {
        m_forcedDirection = std::nullopt;
    }

    m_weaponUseTimeLeft -= dt;
    m_weaponCooldownLeft -= dt;

    m_effects.update(dt);
}
void Player::processAsyncKeyboardInput(World& world, float dt, const ls::Vec2I& mousePos)
{
    constexpr float acc = 200.0f / 32.0f;
    float drag = world.drag(m_playerEntity.model().position());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_playerEntity.model().accelerate(Vec2F(-acc * dt * drag, 0.0f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_playerEntity.model().accelerate(Vec2F(acc * dt * drag, 0.0f));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_playerEntity.model().accelerate(Vec2F(0.0f, -acc * dt * drag));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_playerEntity.model().accelerate(Vec2F(0.0f, acc * dt * drag));

    if (world.isWithinWorldWindow(mousePos))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
            && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)
            && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)
            && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)
            && m_weaponCooldownLeft <= 0.0f
            )
        {
            attack(world, world.screenToWorld(mousePos));
        }
    }
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

World& Player::world()
{
    return *m_world;
}
const World& Player::world() const
{
    return *m_world;
}

void Player::setWorld(World& world)
{
    m_world = &world;
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
    return m_equipmentInventory.at(PlayerEquipmentInventory::SlotType::Weapon);
}
const TileStack& Player::lightSource() const
{
    return m_equipmentInventory.at(PlayerEquipmentInventory::SlotType::LightSource);
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

bool Player::addEffect(const Effect& effect)
{
    return m_effects.addEffect(effect);
}
bool Player::hasEffect(int id) const
{
    return m_effects.hasEffect(id);
}
bool Player::removeEffect(int id)
{
    return m_effects.removeEffect(id);
}

void Player::attack(World& world, const ls::Vec2F& pos)
{
    TileStack& weapon = this->weapon();
    if (weapon.isEmpty()) return;

    auto attackResult = weapon.attack(world, *this, pos);

    const int ammoSlot = m_equipmentInventory.slotId(PlayerEquipmentInventory::SlotType::Ammo);
    const int weaponSlot = m_equipmentInventory.slotId(PlayerEquipmentInventory::SlotType::Weapon);
    if (attackResult.ammoUsed > 0) m_equipmentInventory.removeTiles(ammoSlot, attackResult.ammoUsed);
    if (attackResult.weaponUsed > 0) m_equipmentInventory.removeTiles(weaponSlot, attackResult.weaponUsed);

    m_weaponUseTimeLeft = attackResult.useTime;
    m_weaponCooldownLeft = attackResult.cooldown;

    if (attackResult.useTime > 0.0f)
    {
        m_forcedDirection = directionFromOffset(pos - m_playerEntity.model().position());
    }
}

bool Player::tryPlaceTileUnderNearby(TileStack&& tileStack)
{
    const ls::Vec2I worldPos = m_world->worldToTile(m_playerEntity.model().position());
    return m_world->tryPlaceTile(std::move(tileStack), worldPos.x, worldPos.y);
}

const AttributeArray& Player::attributes() const
{
    return m_currentAttributes;
}
void Player::updateAttributes()
{
    m_currentAttributes.clear();

    const int inventorySize = m_equipmentInventory.size();
    for (int i = 0; i < inventorySize; ++i)
    {
        if (m_equipmentInventory.isInCorrectSlot(i))
        {
            const AttributeSet& tileAttributes = m_equipmentInventory.at(i).tile().model().attributes();
            m_currentAttributes += tileAttributes;
        }
    }

    m_currentAttributes += m_effects.attributes();
}
void Player::updateEquipedTiles()
{
    for (TileStack& tileStack : m_equipmentInventory.contents())
    {
        if (tileStack.isEmpty()) continue;

        Tile& tile = tileStack.tile();
        tile.updateEquiped(*this, tileStack.quantity());
    }
}

std::optional<LightParams> Player::light() const
{
    const auto defaultLight = []() {
        return OscillatingLightSource(
            LightParams(1.0f, sf::Color::Red),
            LightParams(1.5f, sf::Color::Blue),
            0.5
        ).now();
    };

    const TileStack& source = lightSource();
    if (source.isEmpty())
    {
        return defaultLight();
    }

    std::optional<Light> tileLight = source.tile().model().light({ 0, 0 }); //position does not matter
    if (tileLight.has_value())
    {
        return tileLight.value().params();
    }
    else
    {
        return defaultLight();
    }
}

EntityModel::Direction Player::directionFromOffset(const ls::Vec2F& offset) const
{
    const float ax = std::abs(offset.x);
    const float ay = std::abs(offset.y);

    if (ax > ay) return offset.x > 0 ? EntityModel::Direction::East : EntityModel::Direction::West;
    else return offset.y > 0 ? EntityModel::Direction::South : EntityModel::Direction::North;
}

const std::optional<EntityModel::Direction>& Player::forcedDirection() const
{
    return m_forcedDirection;
}