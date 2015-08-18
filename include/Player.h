#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerEquipmentInventory.h"

#include <memory>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Entity;
class World;
class Inventory;
class PlayerUi;
class AttemptToInteractWithExternalInventory;

class Player
{
public:
    Player();

    Entity* createPlayerEntity();
    PlayerUi* createPlayerUi();

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World* world, float dt);

    bool tryOpenInventory(Inventory* inventory, int x, int y);
    void closeInventory(Inventory* inventory);
    bool isInventoryOpened(Inventory* inventory);

    void onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event);

    PlayerEquipmentInventory& equipmentInventory();

protected:
    struct ExternalInventory
    {
        Inventory* inventory;
        int x;
        int y;

        bool operator==(const Inventory* inv) {return inv == inventory;}
    };

    Entity* m_playerEntity;
    PlayerUi* m_playerUi; //will be managed by Root

    PlayerEquipmentInventory m_equipmentInventory;
    std::vector<ExternalInventory> m_externalInventories;
};

#endif // PLAYER_H
