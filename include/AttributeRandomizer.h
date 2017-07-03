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
        double probability;
        int min;
        int max;
        // value = min + (max - min) * x^exponent, where x is uniformly distributed on [0, 1]
    };

private:
    std::vector<AttributeRandomizationParameters> m_parameters;
public:
    void loadFromConfiguration(ConfigurationNode& config);
    AttributeSet randomize() const;

private:
    Attribute randomize(const AttributeRandomizationParameters& params) const;
};