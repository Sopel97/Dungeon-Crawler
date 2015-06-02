#ifndef ROOT_H
#define ROOT_H

#include "WindowSpaceManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/Random.h"

#include <memory>

class World;

class Root
{
public:
    Root();

    static Root& instance();

    void tick(float dt);
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    void run();
    void initResourceLoaders();
    void loadAssets();

    void processAsyncKeyboardInput();
    void onKeyPressed(const sf::Event& event);
    void onWindowResized(const sf::Event& event);

    WindowSpaceManager& windowSpaceManager();

    Random::CMWCEngine rng();

protected:
    sf::RenderWindow m_window;
    sf::RenderStates m_renderStates;
    WindowSpaceManager m_windowSpaceManager;
    Random::CMWCEngine m_rng;
    float m_lastFrameTime;

    std::unique_ptr<World> m_world;

    static constexpr float m_tickTime = 1.0f / 60.0f;
    static constexpr size_t m_defaultWindowWidth = 1024;
    static constexpr size_t m_defaultWindowHeight = 768;
};

#endif // ROOT_H
