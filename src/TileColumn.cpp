#include "TileColumn.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

using namespace ls;

const Rectangle2F TileColumn::m_defaultCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));

TileStack TileColumn::m_emptyTile(std::make_unique<Tile>(), 1);

const TileStack& TileColumn::top() const
{
    return m_tiles.back();
}
TileStack& TileColumn::top()
{
    return m_tiles.back();
}

void TileColumn::placeOnTop(TileStack&& tile)
{
    m_tiles.emplace_back(std::move(tile));
}

TileStack TileColumn::takeFromTop()
{
    TileStack tileStack = std::move(m_tiles.back());
    m_tiles.pop_back();
    return std::move(tileStack);
}

const std::vector<TileStack>& TileColumn::tiles() const
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

bool TileColumn::isTall() const
{
    for (const TileStack& tileStack : m_tiles)
        if (tileStack.tile().renderer().isTall()) return true;

    return false;
}

bool TileColumn::hasCollider() const
{
    for (const TileStack& tileStack : m_tiles)
    {
        if (tileStack.tile().model().hasCollider()) return true;
    }

    return false;
}
const Rectangle2F& TileColumn::collider() const
{
    for (const TileStack& tileStack : m_tiles)
    {
        if (tileStack.tile().model().hasCollider()) return tileStack.tile().model().collider();
    }

    return m_defaultCollider;
}

int TileColumn::topZ() const
{
    return m_tiles.size() - 1;
}
