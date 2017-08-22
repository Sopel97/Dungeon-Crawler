#pragma once

#include "Configuration.h"

#include "tiles/TileRarity.h"

class TileRaritySelector
{
public:
    TileRaritySelector();

    static TileRaritySelector createDefault();

    void loadFromConfiguration(ConfigurationNode& config);

    TileRarity select(float quality) const;

protected:
    std::vector<TileRarity> m_rarities;
    std::vector<float> m_thresholdQualities;

    TileRaritySelector(std::vector<TileRarity>&& rarities, std::vector<float>&& thresholds);
};