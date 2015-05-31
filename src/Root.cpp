#include "Root.h"

#include "SFMLUtil.h"
#include "World.h"

Root::Root() :
    m_window(sf::VideoMode(m_defaultWindowWidth, m_defaultWindowHeight), "Dungeon Crawler Test"),
    m_renderStates(),
    m_windowSpaceManager(m_window),
    m_lastFrameTime(0.0f),
    m_world(std::make_unique<World>(*this))
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

    m_window.display();
}

void Root::run()
{
    initResourceLoaders();
    loadAssets();

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
            if(event.type == sf::Event::EventType::Closed) m_window.close();
            if(event.type == sf::Event::EventType::Resized) onWindowResized(event);
        }
        if(elapsedTime.asSeconds() >= m_tickTime + lastTick)
        {
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
void Root::initResourceLoaders()
{
    ResourceManager::instance().registerLoader<TextureLoader>();
}

void Root::loadAssets()
{
    ResourceManager::instance().load<sf::Texture>("assets\\gfx\\spritesheet.png", "Spritesheet");
}

void Root::onWindowResized(const sf::Event& event)
{
    m_windowSpaceManager.onWindowResized(event);
}


void* Root::TextureLoader::load(const std::string& path) const
{
    sf::Texture* texture = new sf::Texture;
    texture->loadFromFile(path);
    return texture;
}
Root::TextureLoader::~TextureLoader()
{

}
