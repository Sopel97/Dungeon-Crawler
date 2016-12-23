#include "WeightedSpriteSet.h"

#include "Root.h"

#include <algorithm>

using namespace ls;

WeightedSpriteSet::WeightedSpriteSet()
{
}

void WeightedSpriteSet::loadFromConfiguration(ConfigurationNode& config)
{
    m_sprites.clear();
    m_cumulativeWeights.clear();

    int numberOfSprites = config.length();
    m_sprites.reserve(numberOfSprites);
    m_cumulativeWeights.reserve(numberOfSprites);

    float weightSum = 0.0f;
    for(int i = 1; i <= numberOfSprites; ++i)
    {
        int x = config[i][1].get<int>();
        int y = config[i][2].get<int>();

        float weight = config[i][3].getDefault<float>(1.0f);
        m_sprites.emplace_back(x, y);
        weightSum += weight;
        m_cumulativeWeights.emplace_back(weightSum);
    }
}


Vec2I WeightedSpriteSet::chooseRandomSprite() const
{
    if(m_cumulativeWeights.size() == 0) return {0, 0};

    float sumOfWeights = m_cumulativeWeights.back();

    std::uniform_real_distribution<float> distr(0.0f, sumOfWeights);
    int spriteIndex = std::lower_bound(m_cumulativeWeights.begin(), m_cumulativeWeights.end(), distr(Root::instance().rng())) - m_cumulativeWeights.begin();

    return m_sprites[spriteIndex];
}

ls::Vec2I WeightedSpriteSet::defaultSprite() const
{
    return m_sprites[0];
}
