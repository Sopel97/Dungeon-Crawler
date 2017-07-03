#include "DamageCalculator.h"

#include <algorithm>

#include "AttributeArray.h"

int DamageCalculator::calculateDamage(const AttributeArray& attacker, const AttributeArray& defender)
{
    return std::max(0, attacker[AttributeId::Attack] - defender[AttributeId::Defense]);
}