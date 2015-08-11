#include "WindowSpaceManager.h"

using namespace Geo;

WindowSpaceManager::Region::Region(const RectangleI& rect, WindowSpaceManager::Region::Id id, const std::vector<WindowSpaceManager::Scene>& parentScenes, int zIndex) :
    m_rect(rect),
    m_id(id),
    m_parentScenes(parentScenes),
    m_zIndex(zIndex)
{

}


Vec2I WindowSpaceManager::Region::localCoords(const Vec2I& windowCoords) const
{
    return windowCoords - m_rect.min;
}
const RectangleI& WindowSpaceManager::Region::rect() const
{
    return m_rect;
}
WindowSpaceManager::Region::Id WindowSpaceManager::Region::id() const
{
    return m_id;
}
const std::vector<WindowSpaceManager::Scene>& WindowSpaceManager::Region::parentScenes() const
{
    return m_parentScenes;
}
bool WindowSpaceManager::Region::isPresentOnScene(WindowSpaceManager::Scene scene) const
{
    return std::find(m_parentScenes.begin(), m_parentScenes.end(), scene) != m_parentScenes.end();
}
int WindowSpaceManager::Region::zIndex() const
{
    return m_zIndex;
}

WindowSpaceManager::WindowSpaceManager(sf::RenderWindow& window) :
    m_window(window),
    m_currentScene(Scene::Main)
{
    updateRegions();
}
void WindowSpaceManager::setViewToRect(const RectangleI& windowViewRect, const RectangleF& worldViewRect)
{
    Vec2F windowSize {(float)m_window.getSize().x, (float)m_window.getSize().y};
    Vec2F windowViewTopLeft = windowViewRect.min;
    Vec2F windowViewSize = windowViewRect.max - windowViewRect.min;
    Vec2F windowViewTopLeftRelativeToWindow = windowViewTopLeft / windowSize;
    Vec2F windowViewSizeRelativeToWindow = windowViewSize / windowSize;

    sf::FloatRect view(windowViewTopLeftRelativeToWindow.x, windowViewTopLeftRelativeToWindow.y, windowViewSizeRelativeToWindow.x, windowViewSizeRelativeToWindow.y);
    sf::View panelView(sf::Vector2f(worldViewRect.min.x + worldViewRect.width() / 2.0f, (worldViewRect.min.y + worldViewRect.height() / 2.0f)), sf::Vector2f(worldViewRect.width(), worldViewRect.height()));
    panelView.setViewport(view);
    m_window.setView(panelView);
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId)
{
    const RectangleI& rect = regionRect(regionId);
    setViewToRect(rect, RectangleI(Vec2I(0, 0), rect.width(), rect.height()));
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId, const RectangleF& worldViewRect)
{
    const RectangleI& rect = regionRect(regionId);
    setViewToRect(rect, worldViewRect);
}

void WindowSpaceManager::setDefaultView()
{
    Vec2F windowSize {(float)m_window.getSize().x, (float)m_window.getSize().y};

    sf::FloatRect view(0.0f, 0.0f, 1.0f, 1.0f);
    sf::View panelView(sf::Vector2f((windowSize.x) / 2.0f, ((windowSize.y) / 2.0f)), sf::Vector2f(windowSize.x, windowSize.y));
    panelView.setViewport(view);
    m_window.setView(panelView);
}
void WindowSpaceManager::updateRegions()
{
    constexpr int playerGuiWidth = 200;

    Vec2I windowSize {(int)m_window.getSize().x, (int)m_window.getSize().y};

    int worldViewSize = std::min(windowSize.x - playerGuiWidth, windowSize.y);

    m_regions[Region::PlayerUi] = Region
    {
        RectangleI
        (
            Vec2I
            (
                windowSize.x - playerGuiWidth,
                0
            ),
            playerGuiWidth,
            windowSize.y
        ),
        Region::PlayerUi,
        std::vector<Scene>
        {
            Scene::Main
        },
        0
    };
    m_regions[Region::World] = Region
    {
        RectangleI
        (
            Vec2I
            (
                (windowSize.x - playerGuiWidth - worldViewSize) / 2,
                (windowSize.y - worldViewSize) / 2
            ),
            worldViewSize,
            worldViewSize
        ),
        Region::World,
        std::vector<Scene>
        {
            Scene::Main
        },
        0
    };
}

WindowSpaceManager::Scene WindowSpaceManager::currentScene() const
{
    return m_currentScene;
}
void WindowSpaceManager::setCurrentScene(Scene scene)
{
    m_currentScene = scene;
}

const std::map<WindowSpaceManager::Region::Id, WindowSpaceManager::Region>& WindowSpaceManager::regions() const
{
    return m_regions;
}
const WindowSpaceManager::Region& WindowSpaceManager::region(WindowSpaceManager::Region::Id regionId) const
{
    return m_regions.at(regionId);
}
const WindowSpaceManager::Region* WindowSpaceManager::pointedRegion(const Vec2I& windowCoords) const
{
    const Region* bestCandidate = nullptr;
    int highestZ = std::numeric_limits<int>::min();
    for(const auto& region : m_regions)
    {
        const Region& candidate = region.second;
        if(!candidate.isPresentOnScene(m_currentScene)) continue;
        if(candidate.zIndex() < highestZ) continue;
        if(candidate.rect().intersects(windowCoords))
        {
            bestCandidate = &candidate;
            highestZ = candidate.zIndex();
        }
    }
    return bestCandidate;
}
const RectangleI& WindowSpaceManager::regionRect(WindowSpaceManager::Region::Id regionId) const
{
    return region(regionId).rect();
}

void WindowSpaceManager::onWindowResized(const sf::Event& windowResizedEvent)
{
    updateRegions();
}
