#include "effects/EffectsSet.h"

bool EffectsSet::addEffect(const Effect& effect)
{
    const auto iter = m_effects.find(effect);
    if (iter == m_effects.end())
    {
        m_effects.emplace_hint(iter, effect);
        return true;
    }

    if (effect.overwrite() == EffectOverwrite::Never) return false;

    if (
        effect.overwrite() == EffectOverwrite::Always || 
        (effect.overwrite() == EffectOverwrite::Longer && isFirstLonger(effect, *iter))
        )
    {
        m_effects.erase(iter);
        m_effects.emplace(effect);
        return true;
    }

    return false;
}
bool EffectsSet::hasEffect(int id) const
{
    return m_effects.find(id) != m_effects.end();
}
bool EffectsSet::removeEffect(int id)
{
    auto iter = m_effects.find(id);
    if (iter != m_effects.end())
    {
        m_effects.erase(iter);
        return true;
    }

    return false;
}
AttributeArray EffectsSet::attributes() const
{
    AttributeArray attr;
    for (const auto& tempAttr : m_effects)
    {
        attr += tempAttr.attributes();
    }
    return attr;
}
void EffectsSet::update(float dt)
{
    for (auto iter = m_effects.begin(); iter != m_effects.end();)
    {
        const auto& element = *iter;
        element.update(dt);

        if (!element.stillActive())
        {
            iter = m_effects.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool EffectsSet::isFirstLonger(const Effect& first, const Effect& second)
{
    const auto& firstTimeLeft = first.timeLeft();
    const auto& secondTimeLeft = second.timeLeft();

    if (!firstTimeLeft.has_value()) return true;
    if (!secondTimeLeft.has_value()) return false;

    return firstTimeLeft.value() > secondTimeLeft.value();
}