#include "..\include\WindowSpaceUser.h"


WindowSpaceUser::WindowSpaceUser(const WindowSpaceManager::WindowFullLocalization& loc) :
	m_windowSpaceManager(loc.windowSpaceManager),
	m_window(loc.window)
{
	m_window->setUser(*this);
}
const WindowSpaceManager::Window& WindowSpaceUser::window() const
{
    return *m_window;
}
WindowSpaceManager::Window& WindowSpaceUser::window()
{
    return *m_window;
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
