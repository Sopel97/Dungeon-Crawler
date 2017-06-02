#include "WeightedSpriteSet.h"

#include <algorithm>
#include <random>

#include "Rng.h"

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


int WeightedSpriteSet::chooseRandomSprite() const
{
    if(m_cumulativeWeights.size() == 0) return 0;

    float sumOfWeights = m_cumulativeWeights.back();

    std::uniform_real_distribution<float> distr(0.0f, sumOfWeights);
    int spriteIndex = std::lower_bound(m_cumulativeWeights.begin(), m_cumulativeWeights.end(), distr(Rng<std::ranlux48>::instance().rng())) - m_cumulativeWeights.begin();

    return spriteIndex;
}

int WeightedSpriteSet::defaultSprite() const
{
    return 0;
}
const ls::Vec2I& WeightedSpriteSet::at(int i) const
{
    return m_sprites[i];
}
