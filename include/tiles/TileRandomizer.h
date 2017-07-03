#pragma once

#include <vector>

#include "ResourceManager.h"
#include "TilePrefab.h"
#include "TileStack.h"
#include "Configuration.h"

class Inventory;

class TileRandomizer
{
private:
    struct TileRandomizationParameters;

    struct TileRandomizationChoice
    {
        ResourceHandle<TilePrefab> tilePrefab;
        double exponent;
        double weight;
        int min;
        int max;
        std::vector<TileRandomizationParameters> children;
    };

    struct TileRandomizationParameters
    {
        double probability;
        int numToChoose;
        std::vector<TileRandomizationChoice> choices;
    };
    

private:
    std::vector<TileRandomizationParameters> m_parameters;
public:
    void loadFromConfiguration(ConfigurationNode& config);
    std::vector<TileStack> randomize() const;
    void randomize(Inventory& targetInventory) const;

private:
    std::vector<TileStack> randomize(const TileRandomizationParameters& params) const;
    TileStack randomize(const TileRandomizationChoice& choice) const;
    std::vector<TileRandomizationParameters> loadFromConfigurationPartial(ConfigurationNode& config);
};