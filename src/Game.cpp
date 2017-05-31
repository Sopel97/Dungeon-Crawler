#include "Game.h"

#include "Root.h"
#include "window/WindowSpaceManager.h"
#include "window/InternalWindow.h"
#include "RectSubdivision.h"
#include "PlayerUi.h"

#include "SFMLUtil.h"

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>

using namespace ls;

Game::Game(Root& root) :
    m_root(root),
    m_player(nullptr),
    m_world(nullptr)
{
    WindowSpaceManager& wsm = root.windowSpaceManager();
    auto& scene = wsm.createScene("MainScene");
    auto regions = scene.subdivide(
        scene.rootHandle(),
        RectSubdivision::withPixels(
            RectSubdivision::Orientation::Horizontal,
            RectSubdivision::Subject::Second,
            PlayerUi::playerUiPanelWidth()
        ).withAspectRatio(
            AspectRatio(1.0f),
            RectSubdivision::Subject::First
        ),
        "World",
        "PlayerUi"
    );
    wsm.changeScene("MainScene");

    m_player = std::make_unique<Player>(wsm, *this, m_tileTransferMediator);
    m_world = std::make_unique<World>(root, *m_player, m_tileTransferMediator);

    regions.second.data().attachContent(m_player->playerUi());
    regions.first.data().attachContent(*m_world);
}

Player& Game::player()
{
    return *m_player;
}
World& Game::world()
{
    return *m_world;
}

void Game::tick(float dt)
{
    m_world->update(dt);
}

void Game::draw(sf::RenderStates& renderStates)
{
    WindowSpaceManager& windowSpaceManager = m_root.windowSpaceManager();

    //sf::VertexArray vertexBuffer(sf::PrimitiveType::Triangles);
    //SFMLUtil::appendAsTriangleList(vertexBuffer, Rectangle2F(windowSpaceManager.regionRect(WindowSpaceManager::Region::World)), sf::Color::Red);
    //SFMLUtil::appendAsTriangleList(vertexBuffer, Rectangle2F(windowSpaceManager.regionRect(WindowSpaceManager::Region::PlayerUi)), sf::Color::Green);

    //windowSpaceManager.setDefaultView();
    //renderTarget.draw(vertexBuffer, renderStates);

    windowSpaceManager.drawCurrentScene(renderStates);

    windowSpaceManager.setDefaultView();

    sf::Text fpsText(sf::String(std::to_string(m_root.lastMeasuredFps())), m_root.defaultFont(), 20);
    fpsText.setPosition(sf::Vector2f(4.0f, 4.0f));
    fpsText.setColor(sf::Color::White);
    windowSpaceManager.window().draw(fpsText, renderStates);
}
