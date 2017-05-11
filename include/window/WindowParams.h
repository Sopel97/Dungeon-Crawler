#pragma once

#include <optional>

struct WindowParams
{
    int minWindowWidth;
    int minWindowHeight;
    int minContentWidth;
    int minContentHeight;

    std::optional<int> maxWindowWidth;
    std::optional<int> maxWindowHeight;
    std::optional<int> maxContentWidth;
    std::optional<int> maxContentHeight;

    bool isMinimizable;
    bool isCloseable;
    bool isResizeable;
    bool isMovable;
    bool isContentOnly;
    bool hasHeader;
    bool hasScrollBar;
};