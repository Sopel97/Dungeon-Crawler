#pragma once

#include "WindowSpaceManager.h"
#include "SfmlEventHandler.h"

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class WindowContent : public SfmlEventHandler
{
private:
	WindowSpaceManager::Window* m_window;

protected:
    const WindowSpaceManager::Window& window() const;
	WindowSpaceManager::Window& window();
public:
	WindowContent(WindowSpaceManager::Window& wnd);
	virtual ~WindowContent();

    WindowContent(const WindowContent&) = delete;
    WindowContent(WindowContent&& other);
    WindowContent& operator=(const WindowContent&) = delete;
    WindowContent& operator=(WindowContent&& other);

    void detach();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) {};
};

