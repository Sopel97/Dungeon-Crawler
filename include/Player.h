#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "PlayerUi.h"
#include "InventorySystem.h"

#include "AttributeArray.h"

#include "entities/Entity.h"

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

    void update();

    void processAsyncKeyboardInput(World& world, float dt);

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

    void attack(World& world, const ls::Vec2F& pos);

    bool tryPlaceTileUnderNearby(TileStack&& tileStack);

protected:
    WindowSpaceManager* m_wsm;
    World* m_world;
    Entity m_playerEntity;
    PlayerUi m_playerUi;
    InventorySystem m_inventorySystem;
    PlayerEquipmentInventory m_equipmentInventory;

    AttributeArray m_currentAttributes;

    void updateAttributes();

private:
};

#endif // PLAYER_H
