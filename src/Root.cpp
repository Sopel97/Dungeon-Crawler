#include "Root.h"

#include "Game.h"

#include "SFMLUtil.h"
#include "World.h"

#include "tiles/Tile.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include <windows.h>


using namespace ls;

Root::Root() :
	m_window(sf::VideoMode(m_defaultWindowWidth, m_defaultWindowHeight), "Dungeon Crawler Test"),
	m_renderStates(),
	m_windowSpaceManager(m_window),
    m_lastFrameTime(0.0f),
    m_lastMeasuredFps(0),
    m_currentFpsCounter(0),
    m_game(nullptr)
{
    loadAssets();

    m_defaultFont = ResourceManager<sf::Font>::instance().get("Font");
}

Root& Root::instance()
{
    static Root root;
    return root;
}

void Root::run()
{
    m_game = std::make_unique<Game>(*this);

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

        // BUG: crashes after making some event and going out of the window??????????
        while(m_window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
            {
                m_window.close();
            }
            else if (event.type == sf::Event::EventType::Resized)
            {
                onWindowResized(event);
            }
			else
			{
				m_windowSpaceManager.tryDispatchEvent(event);
			}
        }
        if(time >= m_tickTime + lastTick)
        {
            if(m_window.hasFocus()) processAsyncKeyboardInput(dt);
            m_game->tick(dt);
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
            m_window.clear(sf::Color::Black);
            m_game->draw(m_renderStates); //temporarly not capped just to see how fps is doing
            m_window.display();

            ++m_currentFpsCounter;
            lastDraw = time;
        }

    }

}
void Root::processAsyncKeyboardInput(float dt)
{
    const auto mousePos = sf::Mouse::getPosition(m_window);

    m_game->player().processAsyncKeyboardInput(m_game->world(), dt, ls::Vec2I(mousePos.x, mousePos.y));
}

void Root::loadAssets()
{
    loadTextures();
    loadProjectiles();
    loadTiles();
    loadEntities();

    ResourceManager<sf::Font>::instance().loadWithName("Font", "assets\\fonts\\standard_font2.ttf");
}
void Root::loadEffects()
{
    for (const auto& effectPath : scanForFiles("assets\\effects\\", "*.effect"))
    {
        ResourceManager<EffectPrefab>::instance().load(effectPath);
    }
}
void Root::loadTextures()
{
    Configuration config("assets/gfx/textures.meta");
    ConfigurationNode textureList = config["textures"];

    const int numEntries = textureList.length();
    for (int i = 1; i <= numEntries; ++i)
    {
        const std::string path = textureList[i]["path"].get<std::string>();
        const std::string name = textureList[i]["name"].get<std::string>();
        const int gridSize = textureList[i]["gridSize"].getDefault<int>(1);
        const int padding = textureList[i]["padding"].getDefault<int>(0);
        const bool isRepeated = textureList[i]["repeated"].getDefault<bool>(false);
        ResourceHandle<Spritesheet> texture = ResourceManager<Spritesheet>::instance().loadWithName(name, std::string("assets/gfx/") + path, gridSize, padding, isRepeated);
    }
}
void Root::loadProjectiles()
{
    for (const auto& projectilePath : scanForFiles("assets\\projectiles\\", "*.proj"))
    {
        ResourceManager<ProjectilePrefab>::instance().load(projectilePath);
    }
}
void Root::loadTiles()
{
    for (const auto& tilePath : scanForFiles("assets\\tiles\\", "*.tile"))
    {
        ResourceManager<TilePrefab>::instance().load(tilePath);
    }
}
void Root::loadEntities()
{
    for (const auto& entityPath : scanForFiles("assets\\entities\\", "*.ent"))
    {
        ResourceManager<EntityPrefab>::instance().load(entityPath);
    }
}

void Root::onWindowResized(sf::Event& event)
{
    m_windowSpaceManager.onWindowResized();
}

WindowSpaceManager& Root::windowSpaceManager()
{
    return m_windowSpaceManager;
}
sf::RenderWindow& Root::window()
{
    return m_window;
}
int Root::lastMeasuredFps() const
{
    return m_lastMeasuredFps;
}
const sf::Font& Root::defaultFont() const
{
    return m_defaultFont.get();
}
Game& Root::game()
{
    return *m_game;
}

std::vector<std::string> Root::scanForFiles(const std::string& path, const std::string& query) const
{
    std::vector<std::string> files;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    std::wstring fullPath = stringToWString(path + query);
    hFind = FindFirstFile(fullPath.c_str(), &findFileData);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        files.push_back(path + wstringToString(findFileData.cFileName));
    }
    while(FindNextFile(hFind, &findFileData))
    {
        files.push_back(path + wstringToString(findFileData.cFileName));
    }
    FindClose(hFind);
    return files;
}
