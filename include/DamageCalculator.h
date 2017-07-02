#pragma once

class TileAttributeArray;

class DamageCalculator
{
public:
    static int calculateDamage(const TileAttributeArray& attacker, const TileAttributeArray& defender);
};