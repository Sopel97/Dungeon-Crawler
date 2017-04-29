#include "..\include\WindowSpaceUser.h"


WindowSpaceUser::WindowSpaceUser(WindowSpaceManager::WindowRegionFullLocalization& loc) :
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
WindowSpaceManager::WindowRegion& WindowSpaceUser::windowRegion()
{
	return m_scene->windowRegion(m_windowRegionHandle);
}
WindowSpaceUser::~WindowSpaceUser()
{
}
