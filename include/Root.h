#ifndef ROOT_H
#define ROOT_H

#include "WindowSpaceManager.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ResourceManager.h"

#include "Player.h"
#include "PlayerUi.h"

#include <SFML/Window/Event.hpp>

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
    void loadAssets();

    void processAsyncKeyboardInput(float dt);
    void onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
    void onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
    void onMouseMoved(sf::Event::MouseMoveEvent& event);
    void onWindowResized(sf::Event& event);

    WindowSpaceManager& windowSpaceManager();
    sf::RenderWindow& window();
    int lastMeasuredFps() const;
    const sf::Font& defaultFont() const;

    ls::StandardRandomNumberGeneratorWrapper<std::minstd_rand>& rng();

    std::vector<std::string> scanForFiles(const std::string& path, const std::string& query) const;

protected:
    sf::RenderWindow m_window;
    sf::RenderStates m_renderStates;
    std::unique_ptr<WindowSpaceManager> m_windowSpaceManager;
    ls::StandardRandomNumberGeneratorWrapper<std::minstd_rand> m_rng;
    float m_lastFrameTime;
    int m_lastMeasuredFps;
    int m_currentFpsCounter;
    ResourceHandle<sf::Font> m_defaultFont;

    std::unique_ptr<Game> m_game;

    static constexpr float m_tickTime = 1.0f / 60.0f;
    static constexpr size_t m_defaultWindowWidth = 1024;
    static constexpr size_t m_defaultWindowHeight = 768;
private:
    std::wstring stringToWString(const std::string &s) const
    {
        return std::wstring(s.begin(), s.end());
    }
    std::string wstringToString(const std::wstring &s) const
    {
        return std::string(s.begin(), s.end());
    }
};

#endif // ROOT_H
