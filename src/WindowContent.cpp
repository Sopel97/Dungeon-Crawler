#include "..\include\WindowContent.h"


WindowContent::WindowContent(WindowSpaceManager::Window& wnd) :
	m_window(&wnd)
{
	m_window->setContent(*this);
}
WindowContent::WindowContent(WindowContent&& other) :
    m_window(other.m_window)
{
    m_window->setContent(*this);
}
WindowContent& WindowContent::operator=(WindowContent&& other)
{
    m_window = other.m_window;

    m_window->setContent(*this);

    return *this;
}
const WindowSpaceManager::Window& WindowContent::window() const
{
    return *m_window;
}
WindowSpaceManager::Window& WindowContent::window()
{
    return *m_window;
}
void WindowContent::detach()
{
    m_window = nullptr;
}
WindowContent::~WindowContent()
{
    m_window->removeContent();
}
