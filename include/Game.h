#ifndef GAME_H
#define GAME_H

#include "InventorySystem.h"

#include "Player.h"
#include "PlayerUi.h"
#include "World.h"

#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Root;
class World;
class WindowSpaceManager;

class Game
{
public:
    Game(Root& root);

    void tick(float dt);
    void draw(sf::RenderStates& renderStates);

    Player& player();

    World& world();

protected:
    Root& m_root;

    Player m_player;
    World m_world;

};

#endif // GAME_H
