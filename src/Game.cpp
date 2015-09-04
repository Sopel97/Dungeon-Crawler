#include "Game.h"

#include "Root.h"
#include "Player.h"
#include "World.h"
#include "WindowSpaceManager.h"

#include "SFMLUtil.h"

#include <SFML/Graphics.hpp>

using namespace ls;

Game::Game(Root& root) :
    m_root(root),
    m_inventorySystem(),
    m_userInputHandler(root, *this)
{
    m_player = std::make_unique<Player>(*this);
    m_world = std::make_unique<World>(root, *m_player);
}

Player& Game::player()
{
    return *m_player;
}
World& Game::world()
{
    return *m_world;
}
InventorySystem& Game::inventorySystem()
{
    return m_inventorySystem;
}

void Game::tick(float dt)
{
    m_world->update(dt);
}

void Game::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    WindowSpaceManager windowSpaceManager = m_root.windowSpaceManager();

    sf::VertexArray vertexBuffer(sf::PrimitiveType::Triangles);
    SFMLUtil::appendAsTriangleList(vertexBuffer, RectangleF(windowSpaceManager.regionRect(WindowSpaceManager::Region::World)), sf::Color::Red);
    SFMLUtil::appendAsTriangleList(vertexBuffer, RectangleF(windowSpaceManager.regionRect(WindowSpaceManager::Region::PlayerUi)), sf::Color::Green);

    windowSpaceManager.setDefaultView();
    renderTarget.draw(vertexBuffer, renderStates);

    m_world->draw(renderTarget, renderStates);

    m_player->playerUi().draw(renderTarget, renderStates);

    windowSpaceManager.setDefaultView();
    sf::Text fpsText(sf::String(std::to_string(m_root.lastMeasuredFps())), m_root.defaultFont(), 20);
    fpsText.setPosition(sf::Vector2f(4.0f, 4.0f));
    fpsText.setColor(sf::Color::White);
    renderTarget.draw(fpsText, renderStates);
}

void Game::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{
    m_userInputHandler.onMouseButtonPressed(event);
}
void Game::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{
    m_userInputHandler.onMouseButtonReleased(event);
}
void Game::onMouseMoved(sf::Event::MouseMoveEvent& event)
{
    m_userInputHandler.onMouseMoved(event);
}
