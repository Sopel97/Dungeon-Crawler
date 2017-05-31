#include "window/WindowContent.h"

WindowContent::WindowContent() :
    m_window(nullptr)
{
}
WindowContent::WindowContent(WindowContent&& other)
{
    if (other.m_window != nullptr)
    {
        other.m_window->attachContent(*this);
    }
}
WindowContent& WindowContent::operator=(WindowContent&& other)
{
    if (other.m_window != nullptr)
    {
        other.m_window->attachContent(*this);
    }

    return *this;
}
WindowContent::~WindowContent()
{

}
const InternalWindow& WindowContent::window() const
{
    return *m_window;
}
InternalWindow& WindowContent::window()
{
    return *m_window;
}
bool WindowContent::hasWindow() const
{
    return m_window != nullptr;
}
void WindowContent::onAttached(InternalWindow& wnd)
{
    m_window = &wnd;
}
void WindowContent::onDetached(InternalWindow& wnd)
{
    m_window = nullptr;
}
void WindowContent::onDetachedAndWindowClosing(InternalWindow& wnd)
{
    m_window = nullptr;
}
