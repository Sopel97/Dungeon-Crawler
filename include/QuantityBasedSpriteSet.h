#ifndef QUANTITYBASEDSPRITESET_H
#define QUANTITYBASEDSPRITESET_H

#include <vector>

#include "Configuration.h"

#include "../LibS/Geometry.h"

class QuantityBasedSpriteSet
{
public:
    QuantityBasedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    ls::Vec2I getSprite(int quantity) const;
    ls::Vec2I defaultSprite() const;
protected:
    std::vector<ls::Vec2I> m_sprites;
    std::vector<int> m_thresholdQuantities;
};

#endif // QUANTITYBASEDSPRITESET_H
