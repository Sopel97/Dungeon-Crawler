#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Player.h"

#include "Inventory.h"
#include "InventoryView.h"

PlayerUi::PlayerUi(Player& player) :
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

bool PlayerUi::tryOpenInventory(Inventory* inventory)
{
    InventoryView newInventoryView = inventory->createInventoryView();
    InventoryView& lastInventoryView = m_inventories.back();

    newInventoryView.setOffsetFromTop(lastInventoryView.offsetFromTop() + lastInventoryView.height());

    m_inventories.push_back(std::move(newInventoryView));

    return true; //later will check if there is enough place on the bar
}
void PlayerUi::closeInventory(Inventory* inventory)
{
    int heightToBeRemoved = 0;
    for(auto& inv : m_inventories)
    {
        if(inv.parentInventory() == inventory) heightToBeRemoved += inv.height(); //if it is to be removed
        else //if not, we push it up
        {
            inv.setOffsetFromTop(inv.offsetFromTop() - heightToBeRemoved);
        }
    }

    m_inventories.erase(std::remove_if(m_inventories.begin(), m_inventories.end(), [inventory](const InventoryView& inv){return inv.parentInventory() == inventory;}), m_inventories.end());
}
