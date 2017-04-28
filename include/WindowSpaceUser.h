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

public:
	WindowSpaceUser(WindowSpaceManager& windowSpaceManager, WindowSpaceManager::Scene& scene, WindowSpaceManager::WindowRegionHandle windowRegionHandle);
	virtual ~WindowSpaceUser();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;
};

