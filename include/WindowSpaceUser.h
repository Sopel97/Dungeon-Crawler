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
	WindowSpaceManager::Scene* m_scene;
	WindowSpaceManager::WindowRegion* m_windowRegion;

protected:
	WindowSpaceManager::Scene& scene();
    const WindowSpaceManager::WindowRegion& windowRegion() const;
	WindowSpaceManager::WindowRegion& windowRegion();
    const WindowSpaceManager& windowSpaceManager() const;
    WindowSpaceManager& windowSpaceManager();
public:
	WindowSpaceUser(const WindowSpaceManager::WindowRegionFullLocalization& loc);
	virtual ~WindowSpaceUser();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) {};
};

