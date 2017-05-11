#pragma once

#include <optional>

#include "RectSubdivision.h"
#include "InternalWindow.h"

class BackgroundWindow : public InternalWindow
{
private:
    std::optional<RectSubdivision> m_subdivisionParams;
public:

    static WindowParams defaultParams();

    BackgroundWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name) :
        InternalWindow(wsm, rect, name, defaultParams()),
        m_subdivisionParams(std::nullopt)
    {
    }
    BackgroundWindow(WindowSpaceManager& wsm, const ls::Rectangle2I& rect, const std::string& name, BackgroundWindow& parent) :
        InternalWindow(wsm, rect, name, defaultParams()),
        m_subdivisionParams(std::nullopt)
    {
        setParent(parent);
    }

    const std::optional<RectSubdivision>& params() const
    {
        return m_subdivisionParams;
    }
    std::optional<RectSubdivision>& params()
    {
        return m_subdivisionParams;
    }

    void setParams(const RectSubdivision& params)
    {
        m_subdivisionParams = params;
    }
};