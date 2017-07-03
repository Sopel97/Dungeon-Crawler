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
        std::vector<TileStack> tileStacks = randomize(params);
        if (tileStacks.empty()) continue;

        tiles.insert(tiles.end(), std::make_move_iterator(tileStacks.begin()), std::make_move_iterator(tileStacks.end()));
    }

    return tiles;
}
void TileRandomizer::randomize(Inventory& targetInventory) const
{
    std::vector<TileStack> tiles = randomize();
    const int numTiles = tiles.size();
    const int numSlots = targetInventory.size();
    const int numToInsert = std::min(numTiles, numSlots);
    for (int i = 0; i < numToInsert; ++i)
    {
        targetInventory.at(i) = std::move(tiles[i]);
    }
}
std::vector<TileStack> TileRandomizer::randomize(const TileRandomizer::TileRandomizationParameters& params) const
{
    if (!Rng<std::ranlux48>::instance().doesHappen(params.probability)) return {};

    std::vector<TileStack> result;

    const int numChoices = params.choices.size();
    if (numChoices == 1)
    {
        const TileRandomizationChoice& choice = params.choices.back();

        TileStack stack = randomize(choice);
        if (!stack.isEmpty()) result.emplace_back(std::move(stack));
    }
    else
    {
        std::vector<double> weights(numChoices);
        for (int i = 0; i < numChoices; ++i)
        {
            weights[i] = params.choices[i].weight;
        }
        auto chosenEntries = Rng<std::ranlux48>::instance().weightedChoose(params.choices, weights, params.numToChoose);

        std::vector<TileStack> tiles;
        tiles.reserve(params.numToChoose);
        for (auto entry : chosenEntries)
        {
            TileStack stack = randomize(*entry);
            if (!stack.isEmpty()) result.emplace_back(std::move(stack));
        }
    }

    return result;
}
TileStack TileRandomizer::randomize(const TileRandomizationChoice& choice) const
{
    const int quantity = Rng<std::ranlux48>::instance().sample(choice.min, choice.max, choice.exponent);
    if (quantity == 0) return TileStack();

    TileStack tileStack = TileStack(choice.tilePrefab->instantiate(), quantity);

    if (!choice.children.empty())
    {
        const auto& childrenParams = choice.children;

        Inventory& inventory = *(tileStack.tile().model().inventory());
        int currentSlot = 0;
        for (const auto& params : childrenParams)
        {
            std::vector<TileStack> tileStacks = randomize(params);

            for (auto&& stack : tileStacks)
            {
                inventory.at(currentSlot) = std::move(stack);
                ++currentSlot;
            }
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
        params.probability = entry["probability"].getDefault<double>(1.0);

        if (entry["choices"].exists())
        {
            params.numToChoose = entry["n"].getDefault<int>(1);

            ConfigurationNode choicesConfig = entry["choices"];
            const int numChoices = choicesConfig.length();
            for (int i = 1; i <= numChoices; ++i)
            {
                ConfigurationNode choiceConfig = choicesConfig[i];
                TileRandomizationChoice choice;

                choice.tilePrefab = ResourceManager::instance().get<TilePrefab>(choiceConfig["tileName"].get<std::string>());
                choice.exponent = choiceConfig["exponent"].getDefault<double>(1.0);
                choice.weight = choiceConfig["weight"].getDefault<double>(1.0);
                choice.min = choiceConfig["min"].get<int>();
                choice.max = choiceConfig["max"].get<int>();

                ConfigurationNode childConfig = choiceConfig["children"];
                if (childConfig.exists())
                {
                    choice.children = loadFromConfigurationPartial(childConfig);
                }

                params.choices.push_back(choice);
            }
        }
        else
        {
            TileRandomizationChoice singleChoice;
            singleChoice.weight = 1.0; // does not matter because there is only one choice

            singleChoice.tilePrefab = ResourceManager::instance().get<TilePrefab>(entry["tileName"].get<std::string>());
            singleChoice.exponent = entry["exponent"].getDefault<double>(1.0);
            singleChoice.min = entry["min"].get<int>();
            singleChoice.max = entry["max"].get<int>();

            ConfigurationNode childConfig = entry["children"];
            if (childConfig.exists())
            {
                singleChoice.children = loadFromConfigurationPartial(childConfig);
            }

            params.choices.push_back(singleChoice);
        }

        parameters.emplace_back(params);
    }

    return parameters;
}