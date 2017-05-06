#include "..\include\WindowSpaceUser.h"


WindowSpaceUser::WindowSpaceUser(const WindowSpaceManager::WindowRegionFullLocalization& loc) :
	m_windowSpaceManager(loc.windowSpaceManager),
	m_scene(loc.scene),
	m_windowRegion(loc.windowRegion)
{
	m_windowRegion->setUser(*this);
}
WindowSpaceManager::Scene& WindowSpaceUser::scene()
{
	return *m_scene;
}
const WindowSpaceManager::WindowRegion& WindowSpaceUser::windowRegion() const
{
    return *m_windowRegion;
}
WindowSpaceManager::WindowRegion& WindowSpaceUser::windowRegion()
{
    return *m_windowRegion;
}
const WindowSpaceManager& WindowSpaceUser::windowSpaceManager() const
{
    return *m_windowSpaceManager;
}
WindowSpaceManager& WindowSpaceUser::windowSpaceManager()
{
    return *m_windowSpaceManager;
}
WindowSpaceUser::~WindowSpaceUser()
{
}
