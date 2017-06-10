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

        int weight = thisConfig[1].getDefault<float>(1.0f);

        ConfigurationNode spriteConfig = thisConfig[2];
        TimeAnimatedSprite sprite;
        sprite.loadFromConfiguration(spriteConfig);

        m_sprites.emplace_back(std::move(sprite));
        weightSum += weight;
        m_cumulativeWeights.emplace_back(weightSum);
    }
}


int WeightedRandomSpriteSelector::select() const
{
    if (m_sprites.size() <= 1) return 0;

    float sumOfWeights = m_cumulativeWeights.back();

    std::uniform_real_distribution<float> distr(0.0f, sumOfWeights);
    const float r = distr(Rng<std::ranlux48>::instance().rng());
    int spriteIndex = std::lower_bound(m_cumulativeWeights.begin(), m_cumulativeWeights.end(), r) - m_cumulativeWeights.begin();

    return spriteIndex;
}

int WeightedRandomSpriteSelector::defaultSprite() const
{
    return 0;
}
const TimeAnimatedSprite& WeightedRandomSpriteSelector::at(int i) const
{
    return m_sprites[i];
}
