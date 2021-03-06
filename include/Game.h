#ifndef GAME_H
#define GAME_H

#include "InventorySystem.h"

#include "Player.h"
#include "PlayerUi.h"
#include "World.h"
#include "TileTransferMediator.h"

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
    void draw(const sf::RenderStates& renderStates);

    Player& player();

    World& world();

protected:
    Root& m_root;
    TileTransferMediator m_tileTransferMediator;

    std::unique_ptr<Player> m_player;
    std::unique_ptr<World> m_world;

};

#endif // GAME_H
