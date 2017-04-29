#include "Game.h"

#include "Root.h"
#include "WindowSpaceManager.h"
#include "PlayerUi.h"

#include "SFMLUtil.h"

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>

using namespace ls;

Game::Game(Root& root) :
    m_root(root),
    m_player(*this),
    m_world(root, m_player)
{
	setupWindow(root.windowSpaceManager());
}

Player& Game::player()
{
    return m_player;
}
World& Game::world()
{
    return m_world;
}

void Game::tick(float dt)
{
    m_world.update(dt);
}

void Game::draw(sf::RenderStates& renderStates)
{
    WindowSpaceManager windowSpaceManager = m_root.windowSpaceManager();

    //sf::VertexArray vertexBuffer(sf::PrimitiveType::Triangles);
    //SFMLUtil::appendAsTriangleList(vertexBuffer, Rectangle2F(windowSpaceManager.regionRect(WindowSpaceManager::Region::World)), sf::Color::Red);
    //SFMLUtil::appendAsTriangleList(vertexBuffer, Rectangle2F(windowSpaceManager.regionRect(WindowSpaceManager::Region::PlayerUi)), sf::Color::Green);

    windowSpaceManager.setDefaultView();
    //renderTarget.draw(vertexBuffer, renderStates);

	windowSpaceManager.drawCurrentScene(renderStates)

    windowSpaceManager.setDefaultView();
    
	sf::Text fpsText(sf::String(std::to_string(m_root.lastMeasuredFps())), m_root.defaultFont(), 20);
    fpsText.setPosition(sf::Vector2f(4.0f, 4.0f));
    fpsText.setColor(sf::Color::White);
    renderTarget.draw(fpsText, renderStates);
}
