#pragma once

struct TileAttackResult
{
    int weaponUsed;
    int ammoUsed;

    static TileAttackResult noAction()
    {
        return { 0, 0 };
    }
};