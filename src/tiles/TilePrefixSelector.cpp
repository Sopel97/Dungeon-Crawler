#include "tiles/TilePrefixSelector.h"

#include <algorithm>

TilePrefixSelector::TilePrefixSelector()
{

}
TilePrefixSelector::TilePrefixSelector(std::vector<std::string>&& prefixes, std::vector<float>&& thresholds) :
    m_prefixes(std::move(prefixes)),
    m_thresholdQualities(std::move(thresholds))
{

}

TilePrefixSelector TilePrefixSelector::createDefault()
{
    std::vector<std::string> prefixes{ 
        "Bad", 
        "Ordinary", 
        "Good",
        "Great",
        "Distinguished",
        "Supreme",
        "Legendary",
        "Epic",
        "Otherwordly",
        "Divine",
    };
    std::vector<float> thresholds{ 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };

    return TilePrefixSelector(std::move(prefixes), std::move(thresholds));
}

void TilePrefixSelector::loadFromConfiguration(ConfigurationNode& config)
{
    m_prefixes.clear();
    m_thresholdQualities.clear();

    const int numEntries = config.length();

    for (int i = 1; i <= numEntries; ++i)
    {
        const float threshold = config[i][1].get<float>();
        std::string prefix = config[i][2].get<std::string>();

        m_prefixes.emplace_back(std::move(prefix));
        m_thresholdQualities.emplace_back(threshold);
    }
}
std::string TilePrefixSelector::select(int quality) const
{
    if (m_prefixes.size() <= 1) return m_prefixes[0];

    if (quality < m_thresholdQualities[0]) return "";

    auto iter = std::lower_bound(m_thresholdQualities.begin(), m_thresholdQualities.end(), quality + std::numeric_limits<float>::epsilon()); //+epsilon is to prevent it from choosing the element we need in the end
    --iter; //should always be possible
    int index = iter - m_thresholdQualities.begin();
    return m_prefixes[index];
}