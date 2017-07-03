#include "DamageCalculator.h"

#include <algorithm>

#include "tiles/TileAttributeArray.h"

int DamageCalculator::calculateDamage(const TileAttributeArray& attacker, const TileAttributeArray& defender)
{
    return std::max(0, attacker[TileAttributeId::Attack] - defender[TileAttributeId::Defense]);
}