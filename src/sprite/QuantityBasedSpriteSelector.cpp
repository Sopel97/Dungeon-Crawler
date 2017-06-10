#include "sprite/QuantityBasedSpriteSelector.h"

using namespace ls;

QuantityBasedSpriteSelector::QuantityBasedSpriteSelector()
{

}

void QuantityBasedSpriteSelector::loadFromConfiguration(ConfigurationNode& config)
{
    m_sprites.clear();
    m_thresholdQuantities.clear();

    int defaultQuantity = 1;
    int numberOfSprites = config.length();

    for (int i = 1; i <= numberOfSprites; ++i)
    {
        ConfigurationNode thisConfig = config[i];

        int quantity = thisConfig[1].getDefault<int>(defaultQuantity);

        ConfigurationNode spriteConfig = thisConfig[2];
        TimeAnimatedSprite sprite;
        sprite.loadFromConfiguration(spriteConfig);

        m_sprites.emplace_back(std::move(sprite));
        m_thresholdQuantities.emplace_back(quantity);

        defaultQuantity = quantity + 1;
    }
}

int QuantityBasedSpriteSelector::select(int quantity) const
{
    if (m_sprites.size() <= 1) return 0;

    auto iter = std::lower_bound(m_thresholdQuantities.begin(), m_thresholdQuantities.end(), quantity + 1); //+1 is to prevent it from choosing the element we need in the end
    --iter; //should always be possible
    int index = iter - m_thresholdQuantities.begin();
    return index;
}

int QuantityBasedSpriteSelector::defaultSprite() const
{
    return 0;
}

const TimeAnimatedSprite& QuantityBasedSpriteSelector::at(int i) const
{
    return m_sprites[i];
}