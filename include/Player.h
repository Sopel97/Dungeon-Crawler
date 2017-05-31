#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerUi.h"

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "PlayerUi.h"
#include "InventorySystem.h"

#include "entities/Entity.h"

class Entity;
class World;
class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;
class WindowSpaceManager;
class InternalWindow;
class TileTransferMediator;

class Player
{
public:
    Player(WindowSpaceManager& wsm, Game& game, TileTransferMediator& tileTransferMediator);

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World& world, float dt);

    bool tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location);
    bool tryInteractWithInternalInventory(Inventory& inventory, const InventorySlotView& slot);

    PlayerUi& playerUi();
    const PlayerUi& playerUi() const;

    Entity& entity();
    const Entity& entity() const;

    InventorySystem& inventorySystem();
    const InventorySystem& inventorySystem() const;

protected:
    WindowSpaceManager* m_wsm;
    TileTransferMediator* m_tileTransferMediator;
    Entity m_playerEntity;
    PlayerUi m_playerUi;
    InventorySystem m_inventorySystem;
};

#endif // PLAYER_H
