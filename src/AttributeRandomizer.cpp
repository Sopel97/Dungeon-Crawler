#include "AttributeRandomizer.h"

#include <string>
#include <cmath>
#include <random>

#include "Rng.h"

void AttributeRandomizer::loadFromConfiguration(ConfigurationNode& config)
{
    m_parameters.clear();
    if (!config.exists()) return;

    const int numEntries = config.length();

    m_parameters.reserve(numEntries);
    for (int i = 1; i <= numEntries; ++i)
    {
        ConfigurationNode entry = config[i];

        AttributeRandomizationParameters params;
        params.attributeId = AttributeIdHelper::stringToEnum(entry["attributeId"].get<std::string>());
        params.exponent = entry["exponent"].getDefault<double>(1.0);
        params.probability = entry["probability"].getDefault<double>(1.0);
        params.min = entry["min"].get<int>();
        params.max = entry["max"].get<int>();

        m_parameters.emplace_back(params);
    }
}
AttributeSet AttributeRandomizer::randomize() const
{
    AttributeSet attributes;

    for (const auto& params : m_parameters)
    {
        Attribute attr = randomize(params);
        if (attr.value == 0) continue;

        attributes += attr;
    }

    return attributes;
}
Attribute AttributeRandomizer::randomize(const AttributeRandomizer::AttributeRandomizationParameters& params) const
{
    if (!Rng<std::ranlux48>::instance().doesHappen(params.probability)) return Attribute{ params.attributeId, 0 };

    const int value = Rng<std::ranlux48>::instance().sample(params.min, params.max, params.exponent);
    if(value == 0) return Attribute{ params.attributeId, 0 };

    return Attribute{ params.attributeId, value };
}