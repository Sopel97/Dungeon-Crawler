#ifndef ROOT_H
#define ROOT_H

#include "WindowSpaceManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "ResourceManager.h"

#include "Player.h"
#include "PlayerUi.h"

#include "../LibS/Random.h"

#include <memory>
#include <vector>
#include <random>

class World;
class Game;

class Root
{
public:
    Root();

    static Root& instance();

    void run();
    void initResourceLoaders();
    void loadAssets();

    void processAsyncKeyboardInput(float dt);
    void onKeyPressed(const sf::Event& event);
    void onWindowResized(const sf::Event& event);

    WindowSpaceManager& windowSpaceManager();
    int lastMeasuredFps() const;
    const sf::Font& defaultFont() const;

    ls::StandardRandomNumberGeneratorWrapper<std::minstd_rand>& rng();

    std::vector<std::string> scanForFiles(const std::string& path, const std::string& query) const;

protected:
    sf::RenderWindow m_window;
    sf::RenderStates m_renderStates;
    WindowSpaceManager m_windowSpaceManager;
    ls::StandardRandomNumberGeneratorWrapper<std::minstd_rand> m_rng;
    float m_lastFrameTime;
    int m_lastMeasuredFps;
    int m_currentFpsCounter;
    ResourceHandle<sf::Font> m_defaultFont;

    std::unique_ptr<Game> m_game;

    static constexpr float m_tickTime = 1.0f / 60.0f;
    static constexpr size_t m_defaultWindowWidth = 1024;
    static constexpr size_t m_defaultWindowHeight = 768;
};

#endif // ROOT_H
