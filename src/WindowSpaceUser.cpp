#include "..\include\WindowSpaceUser.h"


WindowSpaceUser::WindowSpaceUser(const WindowSpaceManager::WindowFullLocalization& loc) :
	m_windowSpaceManager(loc.windowSpaceManager),
	m_window(loc.window)
{
	m_window->setUser(*this);
}
WindowSpaceUser::WindowSpaceUser(WindowSpaceUser&& other) :
    m_windowSpaceManager(other.m_windowSpaceManager),
    m_window(other.m_window)
{
    m_window->setUser(*this);
}
WindowSpaceUser& WindowSpaceUser::operator=(WindowSpaceUser&& other)
{
    m_windowSpaceManager = other.m_windowSpaceManager;
    m_window = other.m_window;

    m_window->setUser(*this);

    return *this;
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
void WindowSpaceUser::detach()
{
    m_windowSpaceManager = nullptr;
    m_window = nullptr;
}
WindowSpaceUser::~WindowSpaceUser()
{
    m_window->removeUser();
}
