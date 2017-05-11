#pragma once

#include "InternalWindow.h"
#include "SfmlEventHandler.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}

class WindowContent : public SfmlEventHandler
{
private:
    InternalWindow* m_window;

protected:
    const InternalWindow& window() const;
    InternalWindow& window();
public:
    WindowContent(InternalWindow& wnd);
    virtual ~WindowContent();

    WindowContent(const WindowContent&) = delete;
    WindowContent(WindowContent&& other);
    WindowContent& operator=(const WindowContent&) = delete;
    WindowContent& operator=(WindowContent&& other);

    void detach();

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) {};
};
