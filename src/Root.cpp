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
    m_rng(12341),
    m_lastFrameTime(0.0f),
    m_lastMeasuredFps(0),
    m_currentFpsCounter(0),
    m_game(nullptr)
{
    loadAssets();

    m_defaultFont = ResourceManager::instance().get<sf::Font>("Font");
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

        while(m_window.pollEvent(event))
        {
			if (event.type == sf::Event::EventType::Closed) m_window.close();
			else if (event.type == sf::Event::EventType::Resized) onWindowResized(event);
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
    m_game->player().processAsyncKeyboardInput(m_game->world(), dt);
}

void Root::loadAssets() // TODO: should be done dynamically
{
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\spritesheet.png", "Spritesheet");
    if(ResourceHandle<sf::Texture> texture = ResourceManager::instance().load<sf::Texture>("assets\\gfx\\ui_background.png", "UiBackground"))
    {
        texture->setRepeated(true);
    }
    if(ResourceHandle<sf::Texture> texture = ResourceManager::instance().load<sf::Texture>("assets\\gfx\\ui_vertical_bars.png", "UiVerticalBars"))
    {
        texture->setRepeated(true);
    }
    if(ResourceHandle<sf::Texture> texture = ResourceManager::instance().load<sf::Texture>("assets\\gfx\\ui_horizontal_bars.png", "UiHorizontalBars"))
    {
        texture->setRepeated(true);
    }
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\ui_non_repeating.png", "UiNonRepeating");
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\light_disc.png", "LightDisc");

    for(const auto& tilePath : scanForFiles("assets\\tiles\\", "*.tile"))
    {
        ResourceManager::instance().load<TilePrefab>(tilePath);
    }

    ResourceManager::instance().load<sf::Font>("assets\\fonts\\standard_font.ttf", "Font");
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

std::mt19937& Root::rng()
{
    return m_rng;
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


void Root::setupWindow()
{
	auto& scene = m_windowSpaceManager.createScene("MainScene");
	auto regions = scene.subdivide(
		scene.rootHandle(),
		WindowSpaceManager::SubdivisionParams::withPixels(
			WindowSpaceManager::SubdivisionParams::Orientation::Horizontal,
			WindowSpaceManager::SubdivisionParams::Subject::Second,
			PlayerUi::playerUiPanelWidth()),
		"World",
		"PlayerUi"
	);
}
