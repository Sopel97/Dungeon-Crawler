#include "effects/Effect.h"

Effect::Effect(int id, const AttributeSet& attributes, EffectOverwrite overwrite, std::optional<float> duration) :
    m_id(id),
    m_attributes(attributes),
    m_overwrite(overwrite),
    m_timeLeft(duration)
{

}

int Effect::id() const
{
    return m_id;
}
const AttributeSet& Effect::attributes() const
{
    return m_attributes;
}
EffectOverwrite Effect::overwrite() const
{
    return m_overwrite;
}

const std::optional<float>& Effect::timeLeft() const
{
    return m_timeLeft;
}

bool Effect::stillActive() const
{
    return m_timeLeft.value_or(1.0f) > 0.0f;
}

void Effect::update(float dt) const
{
    if (!m_timeLeft.has_value()) return;

    m_timeLeft.value() -= dt;
}