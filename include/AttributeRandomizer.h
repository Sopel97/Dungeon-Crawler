#pragma once

#include <vector>

#include "AttributeSet.h"
#include "Configuration.h"

class AttributeRandomizer
{
private:
    struct AttributeRandomizationParameters
    {
        AttributeId attributeId;
        double exponent;
        double weight;
        int min;
        int max;
        // value = min + (max - min) * x^exponent, where x is uniformly distributed on [0, 1]
    };

    struct AttributeRandomizationGroup
    {
        double probability;
        int min;
        int max;
        std::vector<AttributeRandomizationParameters> parameters;
    };

private:
    std::vector<AttributeRandomizationGroup> m_groups;
public:
    struct AttributeRandomizationResult
    {
        AttributeSet attributes;
        float quality;
    };

    void loadFromConfiguration(ConfigurationNode& config);
    AttributeRandomizationResult randomize() const;

private:
    Attribute randomize(const AttributeRandomizationParameters& params) const;
    std::vector<AttributeRandomizationParameters> loadChoices(ConfigurationNode& node) const;
};