#pragma once

#include "configuration/Configuration.h"

#include <memory>

#include "Effect.h"

class EffectPrefab
{
public:
    EffectPrefab();
    ~EffectPrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    Effect instantiate(std::optional<float> duration = std::nullopt) const;
protected:
    int m_id;
    AttributeSet m_attributes;
    EffectOverwrite m_overwrite;

    static int m_lastId;
};

