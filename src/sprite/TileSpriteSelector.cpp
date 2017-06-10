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

const TimeAnimatedSprite& TileSpriteSelector::at(int i) const
{
    return std::visit(At{i}, m_spriteSelector);
}

int TileSpriteSelector::onTileInstantiated(int fallbackSprite) const
{
    return std::visit(OnTileInstantiated{fallbackSprite}, m_spriteSelector);
}
int TileSpriteSelector::onTileQuantityChanged(int newQuantity, int fallbackSprite) const
{
    return std::visit(OnTileQuantityChanged{newQuantity, fallbackSprite}, m_spriteSelector);
}

int TileSpriteSelector::OnTileInstantiated::operator()(const QuantityBasedSpriteSelector& selector) const
{
    return selector.select(1);
}
int TileSpriteSelector::OnTileInstantiated::operator()(const WeightedRandomSpriteSelector& selector) const
{
    return selector.select();
}

int TileSpriteSelector::OnTileQuantityChanged::operator()(const QuantityBasedSpriteSelector& selector) const
{
    return selector.select(newQuantity);
}
int TileSpriteSelector::OnTileQuantityChanged::operator()(const WeightedRandomSpriteSelector& selector) const
{
    return fallback;
}

const TimeAnimatedSprite& TileSpriteSelector::At::operator()(const QuantityBasedSpriteSelector& selector) const
{
    return selector.at(i);
}
const TimeAnimatedSprite& TileSpriteSelector::At::operator()(const WeightedRandomSpriteSelector& selector) const
{
    return selector.at(i);
}