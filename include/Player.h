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

class Player
{
public:
    Player(Game& game);

    Entity* createPlayerEntity();

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World& world, float dt);

    bool tryInteractWithExternalInventory(Inventory* inventory, const TileLocation& location);

    PlayerUi& playerUi();
    const PlayerUi& playerUi() const;

    InventorySystem& inventorySystem();
    const InventorySystem& inventorySystem() const;

protected:
    Entity* m_playerEntity;
    PlayerUi m_playerUi;
    InventorySystem m_inventorySystem;
};

#endif // PLAYER_H
