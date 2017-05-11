#pragma once

#include "InternalWindow.h"

class FreeWindow : public InternalWindow
{
public:
    static WindowParams defaultParams();

    FreeWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name) :
        InternalWindow(wsm, rect, name)
    {
    }
};