#include "sprite/TileSpriteSelector.h"


void TileSpriteSelector::loadFromConfiguration(ConfigurationNode& config)
{
    const std::string spriteSelectorType = config["spriteSelector"].getDefault<std::string>("QuantityBased");

    if (spriteSelectorType == "QuantityBased")
    {
        QuantityBasedSpriteSelector selector;
        ConfigurationNode selectorConfig = config["sprites"];
        selector.loadFromConfiguration(selectorConfig);
        m_spriteSelector = std::move(selector);
    }
    else if(spriteSelectorType == "WeightedRandom")
    {
        WeightedRandomSpriteSelector selector;
        ConfigurationNode selectorConfig = config["sprites"];
        selector.loadFromConfiguration(selectorConfig);
        m_spriteSelector = std::move(selector);
    }
    else throw std::runtime_error("Invaid sprite selector specified: " + spriteSelectorType);
}

const TimeAnimatedSprite& TileSpriteSelector::onTileInstantiated() const
{
    return std::visit(OnTileInstantiated{}, m_spriteSelector);
}
const TimeAnimatedSprite& TileSpriteSelector::onTileQuantityChanged(int newQuantity, const TimeAnimatedSprite& fallbackSprite) const
{
    return std::visit(OnTileQuantityChanged{newQuantity, fallbackSprite}, m_spriteSelector);
}

const TimeAnimatedSprite& TileSpriteSelector::OnTileInstantiated::operator()(const QuantityBasedSpriteSelector& selector) const
{
    return selector.select(1);
}
const TimeAnimatedSprite& TileSpriteSelector::OnTileInstantiated::operator()(const WeightedRandomSpriteSelector& selector) const
{
    return selector.select();
}

const TimeAnimatedSprite& TileSpriteSelector::OnTileQuantityChanged::operator()(const QuantityBasedSpriteSelector& selector) const
{
    return selector.select(newQuantity);
}
const TimeAnimatedSprite& TileSpriteSelector::OnTileQuantityChanged::operator()(const WeightedRandomSpriteSelector& selector) const
{
    return fallback;
}