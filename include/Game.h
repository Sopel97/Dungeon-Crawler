#ifndef GAME_H
#define GAME_H

#include "InventorySystem.h"

#include "../LibS/make_unique.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Root;
class World;

class Game
{
public:
    Game(Root& root);

    void tick(float dt);
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    Player& player();
    World& world();
    InventorySystem& inventorySystem();

protected:
    Root& m_root;
    InventorySystem m_inventorySystem;

    std::unique_ptr<Player> m_player;
    std::unique_ptr<World> m_world;
};

#endif // GAME_H
