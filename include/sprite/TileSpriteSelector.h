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

    const TimeAnimatedSprite& onTileInstantiated() const;
    const TimeAnimatedSprite& onTileQuantityChanged(int newQuantity, const TimeAnimatedSprite& fallbackSprite) const;

private:
    std::variant<QuantityBasedSpriteSelector, WeightedRandomSpriteSelector> m_spriteSelector;

    struct OnTileInstantiated
    {
        const TimeAnimatedSprite& operator()(const QuantityBasedSpriteSelector& selector) const;
        const TimeAnimatedSprite& operator()(const WeightedRandomSpriteSelector& selector) const;
    };
    struct OnTileQuantityChanged
    {
        int newQuantity;
        const TimeAnimatedSprite& fallback;

        const TimeAnimatedSprite& operator()(const QuantityBasedSpriteSelector& selector) const;
        const TimeAnimatedSprite& operator()(const WeightedRandomSpriteSelector& selector) const;
    };
};