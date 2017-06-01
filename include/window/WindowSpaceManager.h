#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.h"

class WindowSpaceManager
{
public:

    WindowSpaceManager(sf::RenderWindow& window);
    WindowSpaceManager(const WindowSpaceManager&) = delete;
    WindowSpaceManager(WindowSpaceManager&& other) = delete;
    WindowSpaceManager& operator=(const WindowSpaceManager&) = delete;
    WindowSpaceManager& operator=(WindowSpaceManager&& other) = delete;

    Scene& createScene(const std::string& name);

    Scene& scene(const std::string& name);
    const Scene& scene(const std::string& name) const;

    Scene& currentScene();
    const Scene& currentScene() const;

    void changeScene(const std::string& name);

    ls::Rectangle2I rect() const;

    void onWindowResized();

    sf::View getRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const;
    sf::View getRectView(const ls::Rectangle2F& viewport, const ls::Rectangle2F& worldRect) const;
    sf::View getWindowView(const InternalWindow& window, const ls::Rectangle2F& worldRect) const;
    sf::View getContentView(const InternalWindow& window, const ls::Rectangle2F& worldRect) const;
    void setRectView(const ls::Rectangle2I& viewport, const ls::Rectangle2F& worldRect) const;
    void setRectView(const ls::Rectangle2F& viewport, const ls::Rectangle2F& worldRect) const;
    void setWindowView(const InternalWindow& window, const ls::Rectangle2F& worldRect) const;
    void setContentView(const InternalWindow& window, const ls::Rectangle2F& worldRect) const;
    void setView(const sf::View& view);
    void setDefaultView();

    bool tryDispatchEvent(sf::Event& event);

    void drawCurrentScene(sf::RenderStates& renderStates);

    sf::RenderWindow& window();

private:
    sf::RenderWindow& m_window;
    std::map<std::string, Scene> m_scenes;
    Scene* m_currentScene;

    sf::FloatRect viewportConvertToRatio(const ls::Rectangle2I& rect) const;
};