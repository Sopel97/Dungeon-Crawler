#pragma once

#include <variant>

#include "QuantityBasedSpriteSelector.h"
#include "WeightedRandomSpriteSelector.h"
#include "TimeAnimatedSprite.h"

#include "Configuration.h"

class TileSpriteSelector
{
public:
    TileSpriteSelector() = default;
    void loadFromConfiguration(ConfigurationNode& config);

    const TimeAnimatedSprite& at(int i) const;

    int onTileInstantiated(int fallbackSprite) const;
    int onTileQuantityChanged(int newQuantity, int fallbackSprite) const;

private:
    std::variant<QuantityBasedSpriteSelector, WeightedRandomSpriteSelector> m_spriteSelector;

    struct OnTileInstantiated
    {
        int fallback;

        int operator()(const QuantityBasedSpriteSelector& selector) const;
        int operator()(const WeightedRandomSpriteSelector& selector) const;
    };
    struct OnTileQuantityChanged
    {
        int newQuantity;
        int fallback;

        int operator()(const QuantityBasedSpriteSelector& selector) const;
        int operator()(const WeightedRandomSpriteSelector& selector) const;
    };
    struct At
    {
        int i;

        const TimeAnimatedSprite& operator()(const QuantityBasedSpriteSelector& selector) const;
        const TimeAnimatedSprite& operator()(const WeightedRandomSpriteSelector& selector) const;
    };
};