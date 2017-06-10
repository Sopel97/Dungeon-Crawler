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
        const int thisConfigSize = thisConfig.length();
        TimeAnimatedSprite sprite;
        int quantity;
        if (thisConfigSize == 1)
        {
            quantity = defaultQuantity;
            ConfigurationNode spriteConfig = thisConfig[1];
            sprite.loadFromConfiguration(spriteConfig);
        }
        else // should be 2
        {
            quantity = thisConfig[1].get<int>();
            ConfigurationNode spriteConfig = thisConfig[2];
            sprite.loadFromConfiguration(spriteConfig);
        }


        m_sprites.emplace_back(std::move(sprite));
        m_thresholdQuantities.emplace_back(quantity);

        defaultQuantity = quantity + 1;
    }
}

const TimeAnimatedSprite& QuantityBasedSpriteSelector::select(int quantity) const
{
    if (m_sprites.size() <= 1) return m_sprites[0];

    auto iter = std::lower_bound(m_thresholdQuantities.begin(), m_thresholdQuantities.end(), quantity + 1); //+1 is to prevent it from choosing the element we need in the end
    --iter; //should always be possible
    int index = iter - m_thresholdQuantities.begin();
    return m_sprites[index];
}

const TimeAnimatedSprite& QuantityBasedSpriteSelector::defaultSprite() const
{
    return m_sprites[0];
}