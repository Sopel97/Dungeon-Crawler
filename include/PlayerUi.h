#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Game;

class PlayerUi
{
public:
    PlayerUi(Game& game, Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    Game& m_game;
    Player& m_player;
};

#endif // PLAYERUI_H
