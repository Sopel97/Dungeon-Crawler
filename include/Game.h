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

class Game
{
public:
    Game(Root& root);

    void tick(float dt);
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    void onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
    void onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
    void onMouseMoved(sf::Event::MouseMoveEvent& event);

    Player& player();
    PlayerUi& playerUi();
    World& world();
    InventorySystem& inventorySystem();

protected:
    Root& m_root;

    Player m_player;
    PlayerUi m_playerUi;
    World m_world;

    InventorySystem m_inventorySystem;
};

#endif // GAME_H
