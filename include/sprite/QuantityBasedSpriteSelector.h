#pragma once

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

class QuantityBasedSpriteSelector
{
public:
    QuantityBasedSpriteSelector();

    void loadFromConfiguration(ConfigurationNode& config);

    int select(int quantity) const;
    int defaultSprite() const;

    const ls::Vec2I& at(int i) const;
protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<int> m_thresholdQuantities;
};