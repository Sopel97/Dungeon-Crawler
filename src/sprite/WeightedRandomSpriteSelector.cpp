#include "sprite/WeightedRandomSpriteSelector.h"

#include <algorithm>
#include <random>

#include "Rng.h"

using namespace ls;

WeightedRandomSpriteSelector::WeightedRandomSpriteSelector()
{
}

void WeightedRandomSpriteSelector::loadFromConfiguration(ConfigurationNode& config)
{
    m_sprites.clear();
    m_cumulativeWeights.clear();

    int numberOfSprites = config.length();
    m_sprites.reserve(numberOfSprites);
    m_cumulativeWeights.reserve(numberOfSprites);

    float weightSum = 0.0f;
    for (int i = 1; i <= numberOfSprites; ++i)
    {
        ConfigurationNode thisConfig = config[i];

        const int thisConfigSize = thisConfig.length();
        TimeAnimatedSprite sprite;
        float weight;
        if (thisConfigSize == 1)
        {
            weight = 1.0f;
            ConfigurationNode spriteConfig = thisConfig[1];
            sprite.loadFromConfiguration(spriteConfig);
        }
        else // should be 2
        {
            weight = thisConfig[1].get<float>();
            ConfigurationNode spriteConfig = thisConfig[2];
            sprite.loadFromConfiguration(spriteConfig);
        }

        m_sprites.emplace_back(std::move(sprite));
        weightSum += weight;
        m_cumulativeWeights.emplace_back(weightSum);
    }
}


const TimeAnimatedSprite& WeightedRandomSpriteSelector::select() const
{
    if (m_sprites.size() <= 1) return m_sprites[0];

    float sumOfWeights = m_cumulativeWeights.back();

    std::uniform_real_distribution<float> distr(0.0f, sumOfWeights);
    const float r = distr(Rng<std::ranlux48>::instance().rng());
    int spriteIndex = std::lower_bound(m_cumulativeWeights.begin(), m_cumulativeWeights.end(), r) - m_cumulativeWeights.begin();

    return m_sprites[spriteIndex];
}

const TimeAnimatedSprite& WeightedRandomSpriteSelector::defaultSprite() const
{
    return m_sprites[0];
}
