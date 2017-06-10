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

    int select() const;
    int defaultSprite() const;

    const TimeAnimatedSprite& at(int i) const;

protected:
    std::vector<TimeAnimatedSprite> m_sprites;
    std::vector<float> m_cumulativeWeights;
};
