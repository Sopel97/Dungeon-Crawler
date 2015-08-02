#ifndef WEIGHTEDSPRITESET_H
#define WEIGHTEDSPRITESET_H

#include <vector>

#include "TileLocation.h"

#include "Configuration.h"

#include "../LibS/GeometryLight.h"

class WeightedSpriteSet
{
public:
    WeightedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    Geo::Vec2I chooseRandomSprite(const TileLocation& location) const;


protected:
    std::vector<Geo::Vec2I> m_sprites;
    std::vector<float> m_cumulativeWeights;
};

#endif // WEIGHTEDSPRITESET_H
