#include "Root.h"

#include "SFMLUtil.h"
#include "World.h"

#include "Tile.h"

#include "ResourceLoaders.h"

#include "../LibS/GeometryLight.h"

#include <windows.h>

using namespace Geo;
using namespace Random;

Root::Root() :
    m_window(sf::VideoMode(m_defaultWindowWidth, m_defaultWindowHeight), "Dungeon Crawler Test"),
    m_renderStates(),
    m_windowSpaceManager(m_window),
    m_rng(12341),
    m_lastFrameTime(0.0f),
    m_lastMeasuredFps(0),
    m_currentFpsCounter(0),
    m_world(nullptr)
{
    //ctor
}

Root& Root::instance()
{
    static Root root;
    return root;
}

void Root::tick(float dt)
{
    m_world->update(dt);
}

void Root::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_window.clear(sf::Color::Black);

    sf::VertexArray vertexBuffer(sf::PrimitiveType::Triangles);
    SFMLUtil::appendAsTriangleList(vertexBuffer, m_windowSpaceManager.regionRect(WindowSpaceManager::Region::World), sf::Color::Red);
    SFMLUtil::appendAsTriangleList(vertexBuffer, m_windowSpaceManager.regionRect(WindowSpaceManager::Region::PlayerGui), sf::Color::Green);

    m_windowSpaceManager.setDefaultView();
    m_window.draw(vertexBuffer, m_renderStates);

    m_world->draw(renderTarget, renderStates);

    m_windowSpaceManager.setDefaultView();
    sf::Text fpsText(sf::String(std::to_string(m_lastMeasuredFps)), m_font.get(), 20);
    fpsText.setPosition(sf::Vector2f(4.0f, 4.0f));
    fpsText.setColor(sf::Color::White);
    m_window.draw(fpsText, m_renderStates);

    m_window.display();
}

void Root::run()
{
    initResourceLoaders();
    loadAssets();

    m_font = ResourceManager::instance().get<sf::Font>("Font");
    m_world = std::make_unique<World>(*this);

    sf::Clock clock;
    clock.restart();
    float lastTick = clock.getElapsedTime().asSeconds();
    float lastDraw = lastTick;
    float lastFpsMeasure = lastTick;
    while(m_window.isOpen())
    {
        sf::Event event;

        sf::Time elapsedTime = clock.getElapsedTime();
        float time = elapsedTime.asSeconds();
        float dt = time - lastTick; //delta time is for the tick
        m_lastFrameTime = time - lastDraw;

        while(m_window.pollEvent(event))
        {
            if(event.type == sf::Event::EventType::KeyPressed) onKeyPressed(event);
            if(event.type == sf::Event::EventType::Closed) m_window.close();
            if(event.type == sf::Event::EventType::Resized) onWindowResized(event);
        }
        if(time >= m_tickTime + lastTick)
        {
            if(m_window.hasFocus()) processAsyncKeyboardInput(dt);
            tick(dt);
            lastTick = time;
        }
        if(time >= lastFpsMeasure + 1.0f)
        {
            m_lastMeasuredFps = m_currentFpsCounter;
            m_currentFpsCounter = 0;
            lastFpsMeasure = time;
        }
        if(m_window.hasFocus())
        {
            draw(m_window, m_renderStates); //temporarly not capped just to see how fps is doing
            ++m_currentFpsCounter;
            lastDraw = time;
        }

    }

}
void Root::processAsyncKeyboardInput(float dt)
{
    m_player.processAsyncKeyboardInput(m_world.get(), dt);
}
void Root::initResourceLoaders()
{
    ResourceManager::instance().registerLoader<TextureLoader>();
    ResourceManager::instance().registerLoader<TileLoader>();
    ResourceManager::instance().registerLoader<FontLoader>();
}

void Root::loadAssets()
{
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\spritesheet.png", "Spritesheet");

    for(const auto& tile : scanForFiles("assets\\tiles\\", "*.tile"))
    {
        ResourceManager::instance().load<Tile>(tile);
    }

    ResourceManager::instance().load<sf::Font>("assets\\fonts\\standard_font.ttf", "Font");
}

void Root::onKeyPressed(const sf::Event& event)
{
}
void Root::onWindowResized(const sf::Event& event)
{
    m_windowSpaceManager.onWindowResized(event);
}

WindowSpaceManager& Root::windowSpaceManager()
{
    return m_windowSpaceManager;
}
Player& Root::player()
{
    return m_player;
}

StandardRandomNumberGeneratorWrapper<std::minstd_rand>& Root::rng()
{
    return m_rng;
}

std::vector<std::string> Root::scanForFiles(const std::string& path, const std::string& query) const
{
    std::vector<std::string> files;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    std::string fullPath = path + query;
    hFind = FindFirstFile(fullPath.c_str(), &findFileData);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        files.push_back(path + findFileData.cFileName);
    }
    while(FindNextFile(hFind, &findFileData))
    {
        files.push_back(path + findFileData.cFileName);
    }
    FindClose(hFind);
    return files;
}

