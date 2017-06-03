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
    static std::uniform_real_distribution<double> distr(0.0, 1.0);

    auto& rng = Rng<std::ranlux48>::instance().rng();

    const double probability = params.probability;
    const double r = distr(rng);
    if (r > probability) return TileAttribute{ params.attributeId, 0 };

    const double exponent = params.exponent;
    const double min = params.min - 0.5; //because we round them to the nearest integer later
    const double max = params.max + 0.5;
    
    const double t = std::pow(distr(rng), exponent);
    const double value = min + (max - min)*t;

    int intValue = static_cast<int>(std::round(value));
    if (intValue < min) intValue = min; //may happen due to floating point rounding errors
    if (intValue > max) intValue = max;

    return TileAttribute{ params.attributeId, intValue };
}