#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerUi.h"

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "PlayerUi.h"
#include "InventorySystem.h"

class Entity;
class World;
class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;
class WindowSpaceManager;
class InternalWindow;

class Player
{
public:
    Player(WindowSpaceManager& wsm, Game& game, InternalWindow& wnd);

    Entity* createPlayerEntity();

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World& world, float dt);

    bool tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location);

    PlayerUi& playerUi();
    const PlayerUi& playerUi() const;

    InventorySystem& inventorySystem();
    const InventorySystem& inventorySystem() const;

protected:
    WindowSpaceManager* m_wsm;
    Entity* m_playerEntity;
    PlayerUi m_playerUi;
    InventorySystem m_inventorySystem;
};

#endif // PLAYER_H
