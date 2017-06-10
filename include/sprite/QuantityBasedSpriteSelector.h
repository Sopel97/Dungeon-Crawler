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

    int select(int quantity) const;
    int defaultSprite() const;

    const TimeAnimatedSprite& at(int i) const;
protected:
    std::vector<TimeAnimatedSprite> m_sprites;
    std::vector<int> m_thresholdQuantities;
};