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
	WindowSpaceManager::WindowRegionHandle m_windowRegionHandle;

protected:
	WindowSpaceManager::Scene& scene();
	WindowSpaceManager::WindowRegion& windowRegion();
public:
	WindowSpaceUser(WindowSpaceManager::WindowRegionFullLocalization& loc);
	virtual ~WindowSpaceUser();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) {};
};

