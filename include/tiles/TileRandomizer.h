#pragma once

#include <vector>

#include "ResourceManager.h"
#include "TilePrefab.h"
#include "TileStack.h"
#include "Configuration.h"

class TileRandomizer
{
private:
    struct TileRandomizationParameters
    {
        ResourceHandle<TilePrefab> tilePrefab;
        double exponent;
        double probability;
        int min;
        int max;
        // value = min + (max - min) * x^exponent, where x is uniformly distributed on [0, 1]
    };

private:
    std::vector<TileRandomizationParameters> m_parameters;
public:
    void loadFromConfiguration(ConfigurationNode& config);
    std::vector<TileStack> randomize() const;

private:
    TileStack randomize(const TileRandomizationParameters& params) const;
};