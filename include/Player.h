#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerEquipmentInventory.h"

#include <memory>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Entity;
class World;

class Player
{
public:
    Player();

    Entity* createPlayerEntity();

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World* world, float dt);

    PlayerEquipmentInventory& equipmentInventory();

protected:
    Entity* m_playerEntity;

    PlayerEquipmentInventory m_equipmentInventory;
};

#endif // PLAYER_H
