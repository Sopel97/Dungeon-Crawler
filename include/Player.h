#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "PlayerUi.h"
#include "InventorySystem.h"

#include "AttributeArray.h"
#include "effects/Effect.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

class Entity;
class World;
class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;
class WindowSpaceManager;
class InternalWindow;
class TileTransferMediator;
class InventorySlotLocation;
class World;

class Player
{
public:
    Player(WindowSpaceManager& wsm, Game& game, TileTransferMediator& tileTransferMediator);

    void update(float dt);

    void processAsyncKeyboardInput(World& world, float dt, const ls::Vec2I& mousePos);

    bool tryInteractWithExternalInventory(Tile& tile, Inventory& inventory, const TileLocation& location);
    bool tryInteractWithInternalInventory(Tile& tile, Inventory& inventory, const InventorySlotLocation& slot);

    PlayerUi& playerUi();
    const PlayerUi& playerUi() const;

    Entity& entity();
    const Entity& entity() const;

    World& world();
    const World& world() const;

    const AttributeArray& attributes() const;

    void setWorld(World& world);

    InventorySystem& inventorySystem();
    const InventorySystem& inventorySystem() const;

    TileStack& ammo();
    TileStack& weapon();
    const PlayerEquipmentInventory& equipmentInventory() const;

    void showTileDescription(const TileDescription& description);
    void showTileDescription(TileDescription&& description);

    bool addEffect(const Effect& effect, bool forceOverwrite = false);
    bool hasEffect(int id) const;

    void attack(World& world, const ls::Vec2F& pos);

    bool tryPlaceTileUnderNearby(TileStack&& tileStack);

    const std::optional<EntityModel::Direction>& forcedDirection() const;

protected:
    struct EffectCompareById
    {
        using is_transparent = void;

        bool operator()(const Effect& lhs, const Effect& rhs) const
        {
            return lhs.id() < rhs.id();
        }
        bool operator()(const Effect& lhs, int rhs) const
        {
            return lhs.id() < rhs;
        }
        bool operator()(int lhs, const Effect& rhs) const
        {
            return lhs < rhs.id();
        }
    };

    WindowSpaceManager* m_wsm;
    World* m_world;
    Entity m_playerEntity;
    PlayerUi m_playerUi;
    InventorySystem m_inventorySystem;
    PlayerEquipmentInventory m_equipmentInventory;
    float m_weaponUseTimeLeft;
    float m_weaponCooldownLeft;
    std::optional<EntityModel::Direction> m_forcedDirection;
    std::set<Effect, EffectCompareById> m_temporaryAttributes;

    AttributeArray m_currentAttributes;

    void updateAttributes();
    void updateEquipedTiles();

private:

    EntityModel::Direction directionFromOffset(const ls::Vec2F& offset) const;
};

#endif // PLAYER_H
