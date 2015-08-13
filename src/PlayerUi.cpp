#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Player.h"

#include "Inventory.h"
#include "InventoryView.h"

#include "Root.h"

PlayerUi::PlayerUi(Root& root, Player& player) :
    m_root(root),
    m_player(player)
{
    m_inventories.push_back(m_player.equipmentInventory().createInventoryView());
}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for(auto& inv : m_inventories) inv.draw(renderTarget, renderStates);
}
