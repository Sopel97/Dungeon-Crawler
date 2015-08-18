#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <vector>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Player;
class InventoryView;
class Inventory;

class PlayerUi
{
public:
    PlayerUi(Player& player);
    ~PlayerUi();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    bool tryOpenInventory(Inventory* inventory);
    void closeInventory(Inventory* inventory);
protected:
    Player& m_player;

    std::vector<InventoryView> m_inventories;
};

#endif // PLAYERUI_H
