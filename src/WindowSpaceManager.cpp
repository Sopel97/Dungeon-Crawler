#include "WindowSpaceManager.h"

using namespace ls;

WindowSpaceManager::Region::Region(const Rectangle2I& rect, WindowSpaceManager::Region::Id id, const std::vector<WindowSpaceManager::Scene>& parentScenes, int zIndex) :
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
const Rectangle2I& WindowSpaceManager::Region::rect() const
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

sf::View WindowSpaceManager::viewOfRect(const ls::Rectangle2I& windowViewRect, const ls::Rectangle2F& worldViewRect)
{
    Vec2F windowSize{(float)m_window.getSize().x, (float)m_window.getSize().y};
    Vec2F windowViewTopLeft = Vec2F(windowViewRect.min);
    Vec2F windowViewSize = Vec2F(windowViewRect.max - windowViewRect.min);
    Vec2F windowViewTopLeftRelativeToWindow = windowViewTopLeft / windowSize;
    Vec2F windowViewSizeRelativeToWindow = windowViewSize / windowSize;

    sf::FloatRect view(windowViewTopLeftRelativeToWindow.x, windowViewTopLeftRelativeToWindow.y, windowViewSizeRelativeToWindow.x, windowViewSizeRelativeToWindow.y);
    sf::View panelView(sf::Vector2f(worldViewRect.min.x + worldViewRect.width() / 2.0f, (worldViewRect.min.y + worldViewRect.height() / 2.0f)), sf::Vector2f(worldViewRect.width(), worldViewRect.height()));
    panelView.setViewport(view);

    return panelView;
}
sf::View WindowSpaceManager::viewOfRegion(Region::Id regionId)
{
    const Rectangle2I& rect = regionRect(regionId);
    return viewOfRect(rect, Rectangle2F::withSize(Vec2F(0.0f, 0.0f), static_cast<float>(rect.width()), static_cast<float>(rect.height())));
}
sf::View WindowSpaceManager::viewOfRegion(Region::Id regionId, const ls::Rectangle2F& worldViewRect)
{
    const Rectangle2I& rect = regionRect(regionId);
    return viewOfRect(rect, worldViewRect);
}
void WindowSpaceManager::setViewToRect(const Rectangle2I& windowViewRect, const Rectangle2F& worldViewRect)
{
    m_window.setView(viewOfRect(windowViewRect, worldViewRect));
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId)
{
    m_window.setView(viewOfRegion(regionId));
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId, const Rectangle2F& worldViewRect)
{
    m_window.setView(viewOfRegion(regionId, worldViewRect));
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
    constexpr int playerUiWidth = 230;

    Vec2I windowSize {(int)m_window.getSize().x, (int)m_window.getSize().y};

    int worldViewSize = std::min(windowSize.x - playerUiWidth, windowSize.y);

    m_regions[Region::PlayerUi] = Region
    {
        Rectangle2I::withSize
        (
            Vec2I
            (
                windowSize.x - playerUiWidth,
                0
            ),
            playerUiWidth,
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
        Rectangle2I::withSize
        (
            Vec2I
            (
                (windowSize.x - playerUiWidth - worldViewSize) / 2,
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
        if(ls::intersect(candidate.rect(), windowCoords))
        {
            bestCandidate = &candidate;
            highestZ = candidate.zIndex();
        }
    }
    return bestCandidate;
}
const Rectangle2I& WindowSpaceManager::regionRect(WindowSpaceManager::Region::Id regionId) const
{
    return region(regionId).rect();
}

void WindowSpaceManager::onWindowResized(const sf::Event& windowResizedEvent)
{
    updateRegions();
}
