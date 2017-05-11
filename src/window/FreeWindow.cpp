#include "window/FreeWindow.h"

WindowParams FreeWindow::defaultParams()
{
    WindowParams params;

    params.minWindowWidth = 0;
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = 0;

    params.maxWindowWidth = std::nullopt;
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = std::nullopt;

    params.isMinimizable = false;
    params.isCloseable = true;
    params.isResizeable = false;
    params.isMovable = false;
    params.isContentOnly = false;
    params.hasHeader = true;
    params.hasScrollBar = false;

    return params;
}