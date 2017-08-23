#include "TileColumn.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"

using namespace ls;

TileStack TileColumn::m_emptyTile(std::make_unique<Tile>(), 1);

const TileStack& TileColumn::top() const
{
    return m_tiles.back();
}
TileStack& TileColumn::top()
{
    return m_tiles.back();
}

TileStack& TileColumn::placeOnTop(TileStack&& tile)
{
    m_tiles.emplace_back(std::move(tile));

    return m_tiles.back();
}

TileStack TileColumn::takeFromTop()
{
    TileStack tileStack = std::move(m_tiles.back());
    m_tiles.pop_back();
    return tileStack;
}
TileStack TileColumn::take(int z)
{
    TileStack tileStack = std::move(m_tiles[z]);
    m_tiles.erase(std::next(m_tiles.begin(), z));
    return tileStack;
}

const std::vector<TileStack>& TileColumn::tiles() const
{
    return m_tiles;
}
std::vector<TileStack>& TileColumn::tiles()
{
    return m_tiles;
}

bool TileColumn::isValid(int z) const
{
    return z < m_tiles.size();
}

const TileStack& TileColumn::at(int z) const
{
    if (isValid(z)) return m_tiles[z];
    return m_emptyTile;
}
TileStack& TileColumn::at(int z)
{
    if (isValid(z)) return m_tiles[z];
    return m_emptyTile;
}
int TileColumn::size() const
{
    return m_tiles.size();
}

std::optional<TileCollider> TileColumn::collider(const ls::Vec2I& pos)
{
    for (TileStack& tileStack : m_tiles)
    {
        if (std::optional<TileCollider> collider = tileStack.tile().model().collider(pos)) 
            return collider;
    }

    return std::nullopt;
}
std::optional<ls::Rectangle2F> TileColumn::lightOccluder(const ls::Vec2I& pos) const
{
    for (const TileStack& tileStack : m_tiles)
    {
        if (std::optional<ls::Rectangle2F> lightOccluder = tileStack.tile().model().lightOccluder(pos))
            return lightOccluder;
    }

    return std::nullopt;
}
bool TileColumn::hasAnyLight(const ls::Vec2I& pos) const
{
    for (const TileStack& tileStack : m_tiles)
    {
        if (std::optional<Light> light = tileStack.tile().model().light(pos))
            return true;
    }

    return false;
}

int TileColumn::topZ() const
{
    return m_tiles.size() - 1;
}
