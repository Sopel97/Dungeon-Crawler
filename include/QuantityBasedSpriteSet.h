#ifndef QUANTITYBASEDSPRITESET_H
#define QUANTITYBASEDSPRITESET_H

#include <vector>

#include "configuration/Configuration.h"

#include "../LibS/Geometry.h"

class QuantityBasedSpriteSet
{
public:
    QuantityBasedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    int getSprite(int quantity) const;
    int defaultSprite() const;

    const ls::Vec2I& at(int i) const;
protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<int> m_thresholdQuantities;
};

#endif // QUANTITYBASEDSPRITESET_H
