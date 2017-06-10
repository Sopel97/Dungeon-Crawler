#include "sprite/TimeAnimatedSprite.h"

#include <algorithm>
#include <cmath>

#include "GameTime.h"

TimeAnimatedSprite::TimeAnimatedSprite() :
    m_lastRequest(0)
{
}
void TimeAnimatedSprite::loadFromConfiguration(ConfigurationNode& config)
{
    const int numSprites = config.length();
    if (numSprites == 0) return;

    if (numSprites == 1)
    {
        ConfigurationNode& spriteConfig = config[1];
        const int x = spriteConfig[1].get<int>();
        const int y = spriteConfig[2].get<int>();
        m_frames.push_back(Frame{ {x, y}, 1.0 });
    }
    else
    {
        for (int i = 1; i <= numSprites; ++i)
        {
            ConfigurationNode& spriteConfig = config[1];
            const int x = spriteConfig[1].get<int>();
            const int y = spriteConfig[2].get<int>();
            const double t = spriteConfig[3].get<double>();
            m_frames.push_back(Frame{ { x, y }, t });
        }
    }
}

int TimeAnimatedSprite::now() const
{
    return at(GameTime::instance().now());
}
int TimeAnimatedSprite::at(double time) const
{
    const int numFrames = m_frames.size();
    if (numFrames <= 1) return 0;

    time = std::fmod(time, m_frames.back().endTime);
    if (time < 0.0) return 0; //shouldn't really happen

    // linear search from cached position should be the fastest
    // because a lot of the time the same sprite will be needed
    // and also it will rarely skip any
    for (int i = 0; i < numFrames; ++i) //in case of floating point inaccuracies so we don't loop forever
    {
        const Frame& frame = m_frames[m_lastRequest];
        const double startTime = m_lastRequest == 0 ? 0.0 : m_frames[m_lastRequest - 1].endTime;
        if (time >= startTime && time < frame.endTime) return m_lastRequest;

        ++m_lastRequest;
        if (m_lastRequest >= numFrames) m_lastRequest = 0;
    }

    return 0;
}