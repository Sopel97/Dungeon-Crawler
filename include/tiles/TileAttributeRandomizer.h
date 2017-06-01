#pragma once

#include <vector>
#include <random>

#include "TileAttributeSet.h"
#include "Configuration.h"

class TileAttributeRandomizer
{
private:
    struct AttributeRandomizationParameters
    {
        TileAttributeId attributeId;
        double exponent;
        double probability;
        int min;
        int max;
        // value = min + (max - min) * x^exponent, where x is uniformly distributed on [0, 1]
    };

private:
    static std::ranlux48 m_rng;

    std::vector<AttributeRandomizationParameters> m_parameters;
public:
    void loadFromConfiguration(ConfigurationNode& config);
    TileAttributeSet randomize() const;

private:
    TileAttribute randomize(const AttributeRandomizationParameters& params) const;
};