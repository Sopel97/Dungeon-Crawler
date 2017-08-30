#pragma once

#include <string>
#include <optional>

#include "AttributeSet.h"

#include "Configuration.h"

class Effect
{
public:
    Effect(int id, const AttributeSet& attributes, std::optional<float> duration = std::nullopt);

    int id() const;
    const AttributeSet& attributes() const;
    const std::optional<float>& timeLeft() const;

    bool stillActive() const;

    void update(float dt);

private:
    int m_id;
    AttributeSet m_attributes;
    std::optional<float> m_timeLeft;
};