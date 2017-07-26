#pragma once

#include <vector>

#include "ResourceManager.h"

class TilePrefab;

struct TileUseResult
{
    struct TileCreateRequest
    {
        ResourceHandle<TilePrefab> prefab;
        int numToCreate;
    };

    int numUsed;
    std::vector<TileCreateRequest> tileCreationRequests;

    static TileUseResult noAction()
    {
        return { 0, {} };
    }
};