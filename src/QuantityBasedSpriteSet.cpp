#include "QuantityBasedSpriteSet.h"

using namespace Geo;

QuantityBasedSpriteSet::QuantityBasedSpriteSet()
{

}

void QuantityBasedSpriteSet::loadFromConfiguration(ConfigurationNode& config)
{
    m_sprites.clear();
    m_thresholdQuantities.clear();

    int defaultQuantity = 1;
    int numberOfSprites = config.length();

    for(int i = 1; i <= numberOfSprites; ++i)
    {
        ConfigurationNode spriteConfig = config[i];

        int x = spriteConfig[1].get<int>();
        int y = spriteConfig[2].get<int>();

        int quantity = spriteConfig[3].getDefault<int>(defaultQuantity);

        m_sprites.emplace_back(x, y);
        m_thresholdQuantities.emplace_back(quantity);

        defaultQuantity = quantity + 1;
    }
}

Vec2I QuantityBasedSpriteSet::getSprite(int quantity) const
{
    auto iter = std::lower_bound(m_thresholdQuantities.begin(), m_thresholdQuantities.end(), quantity + 1); //+1 is to prevent it from choosing the element we need in the end
    --iter; //should always be possible
    int index = iter-m_thresholdQuantities.begin();
    return m_sprites[index];
}

Geo::Vec2I QuantityBasedSpriteSet::defaultSprite() const
{
    return m_sprites[0];
}
