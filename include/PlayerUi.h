#ifndef PLAYERUI_H
#define PLAYERUI_H

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Root;

class PlayerUi
{
public:
    PlayerUi(Root& root, Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
protected:
    Root& m_root;
    Player& m_player;
};

#endif // PLAYERUI_H
