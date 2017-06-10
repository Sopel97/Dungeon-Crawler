#pragma once

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "sprite/TimeAnimatedSprite.h"

class WeightedRandomSpriteSelector
{
public:
    WeightedRandomSpriteSelector();

    void loadFromConfiguration(ConfigurationNode& config);

    const TimeAnimatedSprite& select() const;
    const TimeAnimatedSprite& defaultSprite() const;

protected:
    std::vector<TimeAnimatedSprite> m_sprites;
    std::vector<float> m_cumulativeWeights;
};
