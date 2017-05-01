#include "..\include\WindowSpaceUser.h"


WindowSpaceUser::WindowSpaceUser(const WindowSpaceManager::WindowRegionFullLocalization& loc) :
	m_windowSpaceManager(loc.windowSpaceManager),
	m_scene(loc.scene),
	m_windowRegionHandle(loc.regionHandle)
{
	m_scene->windowRegion(m_windowRegionHandle).setUser(*this);
}
WindowSpaceManager::Scene& WindowSpaceUser::scene()
{
	return *m_scene;
}
const WindowSpaceManager::WindowRegion& WindowSpaceUser::windowRegion() const
{
    return m_scene->windowRegion(m_windowRegionHandle);
}
WindowSpaceManager::WindowRegion& WindowSpaceUser::windowRegion()
{
	return m_scene->windowRegion(m_windowRegionHandle);
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
