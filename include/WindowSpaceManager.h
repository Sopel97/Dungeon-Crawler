#ifndef WINDOWSPACEMANAGER_H
#define WINDOWSPACEMANAGER_H

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class WindowSpaceManager
{
public:

    enum class Scene
    {
        Main
    };
    class Region
    {
    public:
        enum Id
        {
            World,
            PlayerUi
        };

        Region() = default;
        Region(const ls::RectangleI& rect, Id id, const std::vector<Scene>& parentScenes, int zIndex = 0);
        Region(const Region&) = default;
        Region(Region&&) = default;

        Region& operator=(const Region&) = default;
        Region& operator=(Region &&) = default;

        ls::Vec2I localCoords(const ls::Vec2I& windowCoords) const;
        const ls::RectangleI& rect() const;
        Id id() const;
        const std::vector<Scene>& parentScenes() const;
        bool isPresentOnScene(Scene scene) const;
        int zIndex() const;
    protected:
        ls::RectangleI m_rect;
        Id m_id;
        std::vector<Scene> m_parentScenes;
        int m_zIndex; //determines priority. Higher means it will be more to the top
    };
public:
    WindowSpaceManager(sf::RenderWindow& window);
    void setViewToRect(const ls::RectangleI& windowViewRect, const ls::RectangleF& worldViewRect);
    void setViewToRegion(Region::Id regionId);
    void setViewToRegion(Region::Id regionId, const ls::RectangleF& worldViewRect);

    void setDefaultView();
    void updateRegions();

    Scene currentScene() const;
    void setCurrentScene(Scene scene);

    const std::map<Region::Id, Region>& regions() const;
    const Region& region(Region::Id regionId) const;
    const Region* pointedRegion(const ls::Vec2I& windowCoords) const;
    const ls::RectangleI& regionRect(Region::Id regionId) const;

    void onWindowResized(const sf::Event& windowResizedEvent);
protected:
    sf::RenderWindow& m_window;
    Scene m_currentScene;

    std::map<Region::Id, Region> m_regions;
};


#endif // WINDOWSPACEMANAGER_H
