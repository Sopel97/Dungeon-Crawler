#pragma once

enum class TileAttributeId
{
    Attack,
    Defense
};

struct TileAttribute
{
    TileAttributeId id;
    int value;
};