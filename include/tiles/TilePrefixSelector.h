#pragma once

#include <string>

#include "configuration/Configuration.h"

class TilePrefixSelector
{
public:
    TilePrefixSelector();

    static TilePrefixSelector createDefault();

    void loadFromConfiguration(ConfigurationNode& config);

    std::string select(float quality) const;

protected:
    std::vector<std::string> m_prefixes;
    std::vector<float> m_thresholdQualities;

    TilePrefixSelector(std::vector<std::string>&& prefixes, std::vector<float>&& thresholds);
};