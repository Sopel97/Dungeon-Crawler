#pragma once

class AttributeArray;

class DamageCalculator
{
public:
    static int calculateDamage(const AttributeArray& attacker, const AttributeArray& defender);
};