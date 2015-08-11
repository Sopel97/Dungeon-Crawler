#include "PlayerUi.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Player.h"

#include "Root.h"

PlayerUi::PlayerUi(Root& root, Player& player) :
    m_root(root),
    m_player(player)
{

}

PlayerUi::~PlayerUi()
{

}

void PlayerUi::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_root.windowSpaceManager().setViewToRegion(WindowSpaceManager::Region::PlayerUi);
}
