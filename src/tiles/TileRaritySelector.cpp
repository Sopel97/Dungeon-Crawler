#include "tiles/TileRaritySelector.h"

#include <algorithm>

TileRaritySelector::TileRaritySelector()
{

}
TileRaritySelector::TileRaritySelector(std::vector<TileRarity>&& rarities, std::vector<float>&& thresholds) :
    m_rarities(std::move(rarities)),
    m_thresholdQualities(std::move(thresholds))
{

}

TileRaritySelector TileRaritySelector::createDefault()
{
    std::vector<TileRarity> rarities{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<float> thresholds{ 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };

    return TileRaritySelector(std::move(rarities), std::move(thresholds));
}

void TileRaritySelector::loadFromConfiguration(ConfigurationNode& config)
{
    m_rarities.clear();
    m_thresholdQualities.clear();

    const int numEntries = config.length();

    for (int i = 1; i <= numEntries; ++i)
    {
        const float threshold = config[i][1].get<float>();
        const int rarity = config[i][2].get<int>();

        m_rarities.emplace_back(rarity);
        m_thresholdQualities.emplace_back(threshold);
    }
}
TileRarity TileRaritySelector::select(int quality) const
{
    if (m_rarities.size() <= 1) return m_rarities[0];

    if (quality < m_thresholdQualities[0]) return TileRarity(0);

    auto iter = std::lower_bound(m_thresholdQualities.begin(), m_thresholdQualities.end(), quality + std::numeric_limits<float>::epsilon()); //+epsilon is to prevent it from choosing the element we need in the end
    --iter; //should always be possible
    int index = iter - m_thresholdQualities.begin();
    return m_rarities[index];
}