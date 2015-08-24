#ifndef WEIGHTEDSPRITESET_H
#define WEIGHTEDSPRITESET_H

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

class WeightedSpriteSet
{
public:
    WeightedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    ls::Vec2I chooseRandomSprite() const;
    ls::Vec2I defaultSprite() const;


protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<float> m_cumulativeWeights;
};

#endif // WEIGHTEDSPRITESET_H
