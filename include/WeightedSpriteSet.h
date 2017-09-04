#ifndef WEIGHTEDSPRITESET_H
#define WEIGHTEDSPRITESET_H

#include <vector>

#include "configuration/Configuration.h"

#include "../LibS/Geometry.h"

class WeightedSpriteSet
{
public:
    WeightedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    int chooseRandomSprite() const;
    int defaultSprite() const;

    const ls::Vec2I& at(int i) const;

protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<float> m_cumulativeWeights;
};

#endif // WEIGHTEDSPRITESET_H
