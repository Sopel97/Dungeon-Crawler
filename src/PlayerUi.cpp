#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Game.h"
#include "Player.h"

PlayerUi::PlayerUi(Game& game, Player& player) :
    m_game(game),
    m_player(player)
{

}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    for(auto& inv : m_game.inventorySystem().openedInventories()) inv->inventoryView.draw(renderTarget, renderStates);
}
