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
    bool hasWindow() const;
public:
    WindowContent();
    ~WindowContent() override;

    WindowContent(const WindowContent&) = delete;
    WindowContent(WindowContent&& other);
    WindowContent& operator=(const WindowContent&) = delete;
    WindowContent& operator=(WindowContent&& other);

    virtual void onAttached(InternalWindow& wnd);
    virtual void onDetached(InternalWindow& wnd);
    virtual void onDetachedAndWindowClosing(InternalWindow& wnd);

    virtual void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) {};
};
