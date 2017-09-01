#pragma once

#include "Effect.h"

#include "AttributeArray.h"

#include <set>

class EffectsSet
{
public:
    bool addEffect(const Effect& effect);
    bool hasEffect(int id) const;
    bool removeEffect(int id);
    AttributeArray attributes() const;
    void update(float dt);

private:
    struct EffectCompareById
    {
        using is_transparent = void;

        bool operator()(const Effect& lhs, const Effect& rhs) const
        {
            return lhs.id() < rhs.id();
        }
        bool operator()(const Effect& lhs, int rhs) const
        {
            return lhs.id() < rhs;
        }
        bool operator()(int lhs, const Effect& rhs) const
        {
            return lhs < rhs.id();
        }
    };

    std::set<Effect, EffectCompareById> m_effects;
};