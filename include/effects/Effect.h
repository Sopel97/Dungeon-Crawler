#pragma once

#include <string>
#include <optional>

#include "AttributeSet.h"

#include "EffectOverwrite.h"

#include "Configuration.h"

class Effect
{
public:
    Effect(int id, const AttributeSet& attributes, EffectOverwrite overwrite, std::optional<float> duration = std::nullopt);

    int id() const;
    const AttributeSet& attributes() const;
    EffectOverwrite overwrite() const;
    const std::optional<float>& timeLeft() const;

    bool stillActive() const;

    void update(float dt) const;

private:
    int m_id;
    AttributeSet m_attributes;
    EffectOverwrite m_overwrite;
    mutable std::optional<float> m_timeLeft; //TODO: make this not mutable
};