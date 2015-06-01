#include "Root.h"

#include "SFMLUtil.h"
#include "World.h"

#include "Tile.h"

#include "ResourceLoaders.h"
#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

using namespace Geo;

Root::Root() :
    m_window(sf::VideoMode(m_defaultWindowWidth, m_defaultWindowHeight), "Dungeon Crawler Test"),
    m_renderStates(),
    m_windowSpaceManager(m_window),
    m_lastFrameTime(0.0f),
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

    m_window.display();
}

void Root::run()
{
    initResourceLoaders();
    loadAssets();

    m_world = std::make_unique<World>(*this);

    sf::Clock clock;
    clock.restart();
    float lastTick = clock.getElapsedTime().asSeconds();
    float lastDraw = lastTick;
    while(m_window.isOpen())
    {
        sf::Event event;

        sf::Time elapsedTime = clock.getElapsedTime();
        int dt = elapsedTime.asSeconds() - lastDraw;
        m_lastFrameTime = dt;

        while(m_window.pollEvent(event))
        {
            if(event.type == sf::Event::EventType::KeyPressed) onKeyPressed(event);
            if(event.type == sf::Event::EventType::Closed) m_window.close();
            if(event.type == sf::Event::EventType::Resized) onWindowResized(event);
        }
        if(elapsedTime.asSeconds() >= m_tickTime + lastTick)
        {
            if(m_window.hasFocus()) processAsyncKeyboardInput();
            tick(dt);
            lastTick = elapsedTime.asSeconds();
        }
        if(m_window.hasFocus())
        {
            draw(m_window, m_renderStates); //temporarly outside just to see how fps is doing
            lastDraw = elapsedTime.asMilliseconds();
        }

    }

}
void Root::processAsyncKeyboardInput()
{
    constexpr float speed = 5.0f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_world->moveCamera(Vec2F(-speed, 0.0f)); //just for testing
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_world->moveCamera(Vec2F(speed, 0.0f));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_world->moveCamera(Vec2F(0.0f, -speed));
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_world->moveCamera(Vec2F(0.0f, speed));
}
void Root::initResourceLoaders()
{
    ResourceManager::instance().registerLoader<TextureLoader>();
    ResourceManager::instance().registerLoader<TileLoader>();
}

void Root::loadAssets()
{
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\spritesheet.png", "Spritesheet");

    ResourceManager::instance().load<Tile>("assets\\tiles\\test.tile");
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

