#pragma once

#include <vector>

#include "../LibS/Shapes.h"

#include "Configuration.h"

class TimeAnimatedSprite
{
    struct Frame
    {
        ls::Vec2I pos;
        double endTime;
    };
private:
    std::vector<Frame> m_frames;
    mutable int m_lastRequest;
public:
    TimeAnimatedSprite();
    void loadFromConfiguration(ConfigurationNode& config);

    int now() const;
    int at(double time) const;
};