#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerUi.h"

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
    Player(Game& game);

    Entity* createPlayerEntity();

    void onKeyPressed(sf::Event::KeyEvent& keyEvent);
    void processAsyncKeyboardInput(World& world, float dt);

protected:
    Entity* m_playerEntity;
};

#endif // PLAYER_H
