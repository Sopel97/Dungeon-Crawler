#include "tiles/TileAttributeRandomizer.h"

#include <string>
#include <cmath>
#include <random>

#include "Rng.h"

void TileAttributeRandomizer::loadFromConfiguration(ConfigurationNode& config)
{
    m_parameters.clear();
    if (!config.exists()) return;

    const int numEntries = config.length();

    m_parameters.reserve(numEntries);
    for (int i = 1; i <= numEntries; ++i)
    {
        ConfigurationNode entry = config[i];

        AttributeRandomizationParameters params;
        params.attributeId = TileAttributeIdHelper::stringToEnum(entry["attributeId"].get<std::string>());
        params.exponent = entry["exponent"].getDefault<double>(1.0);
        params.probability = entry["probability"].getDefault<double>(1.0);
        params.min = entry["min"].get<int>();
        params.max = entry["max"].get<int>();

        m_parameters.emplace_back(params);
    }
}
TileAttributeSet TileAttributeRandomizer::randomize() const
{
    TileAttributeSet attributes;

    for (const auto& params : m_parameters)
    {
        TileAttribute attr = randomize(params);
        if (attr.value == 0) continue;

        attributes += attr;
    }

    return attributes;
}
TileAttribute TileAttributeRandomizer::randomize(const TileAttributeRandomizer::AttributeRandomizationParameters& params) const
{
    const int value = Rng<std::ranlux48>::instance().sample(params.min, params.max, params.exponent, params.probability);
    if(value == 0) return TileAttribute{ params.attributeId, 0 };

    return TileAttribute{ params.attributeId, value };
}