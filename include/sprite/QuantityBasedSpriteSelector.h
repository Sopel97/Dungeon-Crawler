#pragma once

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "sprite/TimeAnimatedSprite.h"

class QuantityBasedSpriteSelector
{
public:
    QuantityBasedSpriteSelector();

    void loadFromConfiguration(ConfigurationNode& config);

    const TimeAnimatedSprite& select(int quantity) const;
    const TimeAnimatedSprite& defaultSprite() const;

protected:
    std::vector<TimeAnimatedSprite> m_sprites;
    std::vector<int> m_thresholdQuantities;
};