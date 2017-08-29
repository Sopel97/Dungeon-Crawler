#pragma once

struct TileAttackResult
{
    int weaponUsed;
    int ammoUsed;
    float useTime;
    float cooldown;

    static TileAttackResult noAction()
    {
        return { 0, 0, 0.0f, 0.0f };
    }
};