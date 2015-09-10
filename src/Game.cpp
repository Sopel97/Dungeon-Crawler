#include "Game.h"

#include "Root.h"
#include "WindowSpaceManager.h"

#include "SFMLUtil.h"

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>

using namespace ls;

Game::Game(Root& root) :
    m_root(root),
    m_player(*this),
    m_playerUi(*this, m_player),
    m_world(root, m_player),
    m_inventorySystem(m_playerUi)
{

}

Player& Game::player()
{
    return m_player;
}
PlayerUi& Game::playerUi()
{
    return m_playerUi;
}
World& Game::world()
{
    return m_world;
}
InventorySystem& Game::inventorySystem()
{
    return m_inventorySystem;
}

void Game::tick(float dt)
{
    m_world.update(dt);
}

void Game::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    WindowSpaceManager windowSpaceManager = m_root.windowSpaceManager();

    sf::VertexArray vertexBuffer(sf::PrimitiveType::Triangles);
    SFMLUtil::appendAsTriangleList(vertexBuffer, RectangleF(windowSpaceManager.regionRect(WindowSpaceManager::Region::World)), sf::Color::Red);
    SFMLUtil::appendAsTriangleList(vertexBuffer, RectangleF(windowSpaceManager.regionRect(WindowSpaceManager::Region::PlayerUi)), sf::Color::Green);

    windowSpaceManager.setDefaultView();
    renderTarget.draw(vertexBuffer, renderStates);

    m_world.draw(renderTarget, renderStates);

    m_playerUi.draw(renderTarget, renderStates);

    windowSpaceManager.setDefaultView();
    sf::Text fpsText(sf::String(std::to_string(m_root.lastMeasuredFps())), m_root.defaultFont(), 20);
    fpsText.setPosition(sf::Vector2f(4.0f, 4.0f));
    fpsText.setColor(sf::Color::White);
    renderTarget.draw(fpsText, renderStates);
}

void Game::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Button::Right)
    {
        const RectangleI& worldViewRect = m_root.windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::World);
        if(Intersections::intersection(worldViewRect, Vec2I(event.x, event.y)))
        {
            const Vec2I tilePosition = m_world.screenToTile(Vec2I(event.x, event.y));
            m_world.useTile(tilePosition);
        }
    }
}
void Game::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{

}
void Game::onMouseMoved(sf::Event::MouseMoveEvent& event)
{

}
