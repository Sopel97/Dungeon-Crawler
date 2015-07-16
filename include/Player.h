#ifndef PLAYER_H
#define PLAYER_H

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

protected:
    Entity* m_playerEntity;
};

#endif // PLAYER_H
