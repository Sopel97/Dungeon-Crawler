#pragma once

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

class WeightedRandomSpriteSelector
{
public:
    WeightedRandomSpriteSelector();

    void loadFromConfiguration(ConfigurationNode& config);

    int select() const;
    int defaultSprite() const;

    const ls::Vec2I& at(int i) const;

protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<float> m_cumulativeWeights;
};
