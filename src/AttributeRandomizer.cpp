#include "AttributeRandomizer.h"

#include <string>
#include <cmath>
#include <random>

#include "Rng.h"

void AttributeRandomizer::loadFromConfiguration(ConfigurationNode& config)
{
    m_groups.clear();
    if (!config.exists()) return;

    const int numEntries = config.length();

    m_groups.reserve(numEntries);
    for (int i = 1; i <= numEntries; ++i)
    {
        ConfigurationNode& groupConfig = config[i];
        const int min = groupConfig["min"].getDefault<int>(1);
        const int max = groupConfig["max"].getDefault<int>(1);
        const double probability = groupConfig["probability"].getDefault<double>(1.0);

        ConfigurationNode& choicesConfig = groupConfig["choices"];
        m_groups.push_back({ probability, min, max, loadChoices(choicesConfig) });
    }
}
AttributeRandomizer::AttributeRandomizationResult AttributeRandomizer::randomize() const
{
    AttributeSet attributes;
    float quality = 0.0f;
    int numAttributes = 0;

    for (const auto& group : m_groups)
    {
        if (!Rng<std::ranlux48>::instance().doesHappen(group.probability)) continue;

        std::vector<float> weights;
        weights.reserve(group.parameters.size());
        for (const auto& param : group.parameters)
        {
            weights.emplace_back(static_cast<float>(param.weight));
        }

        const auto chosen = Rng<std::ranlux48>::instance().weightedChoose(group.parameters, weights, Rng<std::ranlux48>::instance().uniform(group.min, group.max));

        for (const auto* params : chosen)
        {
            Attribute attr = randomize(*params);

            if (params->min == params->max)
            {
                quality += 1.0f;
            }
            else
            {
                quality += static_cast<float>(attr.value - params->min) / static_cast<float>(params->max - params->min + 1);
            }

            attributes += attr;
            ++numAttributes;
        }
    }

    if (numAttributes > 0)
    {
        quality /= numAttributes;
    }

    return { std::move(attributes), quality };
}
std::vector<AttributeRandomizer::AttributeRandomizationParameters> AttributeRandomizer::loadChoices(ConfigurationNode& node) const
{
    const int numEntries = node.length();

    std::vector<AttributeRandomizationParameters> parameters;
    parameters.reserve(numEntries);
    for (int i = 1; i <= numEntries; ++i)
    {
        ConfigurationNode entry = node[i];

        AttributeRandomizationParameters params;
        params.attributeId = AttributeIdHelper::stringToEnum(entry["attributeId"].get<std::string>());
        params.exponent = entry["exponent"].getDefault<double>(1.0);
        params.weight = entry["weight"].getDefault<double>(1.0);
        params.min = entry["min"].get<int>();
        params.max = entry["max"].get<int>();

        parameters.emplace_back(params);
    }

    return parameters;
}
Attribute AttributeRandomizer::randomize(const AttributeRandomizer::AttributeRandomizationParameters& params) const
{
    const int value = Rng<std::ranlux48>::instance().sample(params.min, params.max, params.exponent);

    return Attribute{ params.attributeId, value };
}