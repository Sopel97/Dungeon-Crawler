#ifndef QUANTITYBASEDSPRITESET_H
#define QUANTITYBASEDSPRITESET_H

#include <vector>

#include "Configuration.h"

#include "../LibS/GeometryLight.h"

class QuantityBasedSpriteSet
{
public:
    QuantityBasedSpriteSet();

    void loadFromConfiguration(ConfigurationNode& config);

    Geo::Vec2I getSprite(int quantity) const;
protected:
    std::vector<Geo::Vec2I> m_sprites;
    std::vector<int> m_thresholdQuantities;
};

#endif // QUANTITYBASEDSPRITESET_H
