#pragma once

#include "Configuration.h"

#include <memory>

#include "Effect.h"

class EffectPrefab
{
public:
    EffectPrefab();
    ~EffectPrefab();

    void loadFromConfiguration(Configuration& config);

    Effect instantiate() const;
protected:
    int m_id;
    AttributeSet m_attributes;
    std::optional<float> m_duration;

    static int m_lastId;
};

