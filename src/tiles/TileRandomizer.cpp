#include "tiles/TileRandomizer.h"

#include <string>
#include <cmath>
#include <random>

#include "Rng.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"
#include "Inventory.h"

void TileRandomizer::loadFromConfiguration(ConfigurationNode& config)
{
    m_parameters.clear();
    if (!config.exists()) return;

    m_parameters = loadFromConfigurationPartial(config);
}
std::vector<TileStack> TileRandomizer::randomize() const
{
    std::vector<TileStack> tiles;

    for (const auto& params : m_parameters)
    {
        TileStack tileStack = randomize(params);
        if (tileStack.isEmpty()) continue;

        tiles.emplace_back(std::move(tileStack));
    }

    return tiles;
}
TileStack TileRandomizer::randomize(const TileRandomizer::TileRandomizationParameters& params) const
{
    static std::uniform_real_distribution<double> distr(0.0, 1.0);

    auto& rng = Rng<std::ranlux48>::instance().rng();

    const double probability = params.probability;
    const double r = distr(rng);
    if (r > probability) return TileStack();

    const double exponent = params.exponent;
    const double min = params.min - 0.5; //because we round them to the nearest integer later
    const double max = params.max + 0.5;

    const double t = std::pow(distr(rng), exponent);
    const double q = min + (max - min)*t;

    int quantity = static_cast<int>(std::round(q));
    if (quantity < min) quantity = min; //may happen due to floating point rounding errors
    if (quantity > max) quantity = max;

    TileStack tileStack = TileStack(params.tilePrefab->instantiate(), quantity);

    if (!params.children.empty())
    {
        const std::vector<TileRandomizationParameters>& childrenParams = params.children;

        Inventory& inventory = *(tileStack.tile().model().inventory());
        int currentSlot = 0;
        for (const auto& params : childrenParams)
        {
            TileStack tileStack = randomize(params);
            if (tileStack.isEmpty()) continue;

            inventory.at(currentSlot) = std::move(tileStack);
            ++currentSlot;
        }
    }

    return tileStack;
}
std::vector<TileRandomizer::TileRandomizationParameters> TileRandomizer::loadFromConfigurationPartial(ConfigurationNode& config)
{
    std::vector<TileRandomizationParameters> parameters;

    const int numEntries = config.length();

    parameters.reserve(numEntries);
    for (int i = 1; i <= numEntries; ++i)
    {
        ConfigurationNode entry = config[i];

        TileRandomizationParameters params;
        params.tilePrefab = ResourceManager::instance().get<TilePrefab>(entry["tileName"].get<std::string>());
        params.exponent = entry["exponent"].getDefault<double>(1.0);
        params.probability = entry["probability"].getDefault<double>(1.0);
        params.min = entry["min"].get<int>();
        params.max = entry["max"].get<int>();

        ConfigurationNode childConfig = entry["child"];
        if (childConfig.exists())
        {
            params.children = loadFromConfigurationPartial(childConfig);
        }

        parameters.emplace_back(params);
    }

    return parameters;
}