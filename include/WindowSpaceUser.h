#pragma once

#include "WindowSpaceManager.h"
#include "SfmlEventHandler.h"

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class WindowSpaceUser : public SfmlEventHandler
{
private:
	WindowSpaceManager* m_windowSpaceManager;
	WindowSpaceManager::Window* m_window;

protected:
    const WindowSpaceManager::Window& window() const;
	WindowSpaceManager::Window& window();
    const WindowSpaceManager& windowSpaceManager() const;
    WindowSpaceManager& windowSpaceManager();
public:
	WindowSpaceUser(const WindowSpaceManager::WindowFullLocalization& loc);
	virtual ~WindowSpaceUser();

    WindowSpaceUser(const WindowSpaceUser&) = delete;
    WindowSpaceUser(WindowSpaceUser&& other);
    WindowSpaceUser& operator=(const WindowSpaceUser&) = delete;
    WindowSpaceUser& operator=(WindowSpaceUser&& other);

    void detach();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) {};
};

