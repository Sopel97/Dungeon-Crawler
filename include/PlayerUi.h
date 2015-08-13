#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class Root;
class InventoryView;

class PlayerUi
{
public:
    PlayerUi(Root& root, Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
protected:
    Root& m_root;
    Player& m_player;

    std::vector<InventoryView> m_inventories;
};

#endif // PLAYERUI_H
