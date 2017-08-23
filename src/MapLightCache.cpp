#include "MapLightCache.h"

#include "MapLayer.h"
#include "TileColumn.h"

MapLightCache::MapLightCache(MapLayer& map) :
    m_map(map),
    m_cells((map.width() + m_cellSize - 1) / m_cellSize, (map.height() + m_cellSize - 1) / m_cellSize)
{
}

void MapLightCache::update(const ls::Vec2I& pos)
{
    const ls::Vec2I cellPos = cellCoords(pos);
    LightCacheCell& cell = m_cells(cellPos.x, cellPos.y);
    if (hasAnyLight(pos))
    {
        cell.lightPositions.emplace(pos);
    }
    else
    {
        cell.lightPositions.erase(pos);
    }
}

std::vector<ls::Vec2I> MapLightCache::queryLightPositions(const ls::Rectangle2I& rect) const
{
    static constexpr int numPreallocPositions = 32;

    const ls::Vec2I firstCellPos = cellCoords(rect.min);
    const ls::Vec2I lastCellPos = cellCoords(rect.max);

    std::vector<ls::Vec2I> result;
    result.reserve(numPreallocPositions);

    for (int cellX = firstCellPos.x; cellX <= lastCellPos.x; ++cellX)
    {
        for (int cellY = firstCellPos.y; cellY <= lastCellPos.y; ++cellY)
        {
            for (const auto& pos : m_cells(cellX, cellY).lightPositions)
            {
                if (pos.x >= rect.min.x && pos.x <= rect.max.x && pos.y >= rect.min.y && pos.y <= rect.max.y)
                {
                    result.emplace_back(pos);
                }
            }
        }
    }

    return result;
}

bool MapLightCache::hasAnyLight(const ls::Vec2I& pos) const
{
    const TileColumn& column = m_map.at(pos.x, pos.y);

    return column.hasAnyLight(pos);
}
ls::Vec2I MapLightCache::cellCoords(const ls::Vec2I& pos) const
{
    return pos / m_cellSize;
}