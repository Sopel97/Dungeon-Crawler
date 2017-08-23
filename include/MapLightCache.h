#pragma once

#include <unordered_set>

#include "../LibS/Array2.h"
#include "../LibS/Shapes.h"

class MapLayer;

class MapLightCache
{
public:
    MapLightCache(MapLayer& map);

    void update(const ls::Vec2I& pos);

    std::vector<ls::Vec2I> queryLightPositions(const ls::Rectangle2I& rect) const;

private:

    struct LightCacheCell
    {
        struct CoordsHash
        {
            size_t operator()(const ls::Vec2I& pos) const
            {
                const uint64_t x = pos.x;
                const uint64_t y = pos.y;
                const uint64_t xy = (x << 32ull) + y;

                return std::hash<uint64_t>()(xy);
            }
        };

        std::unordered_set<ls::Vec2I, CoordsHash> lightPositions;
    };

    static constexpr int m_cellSize = 16;

    MapLayer& m_map;
    ls::Array2<LightCacheCell> m_cells;

    bool hasAnyLight(const ls::Vec2I& pos) const;
    ls::Vec2I cellCoords(const ls::Vec2I& pos) const;
};