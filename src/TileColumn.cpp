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
std::optional<Light> TileColumn::light(const ls::Vec2I& pos) const
{
    static constexpr float dimmingCutoffThreshold = 0.9f;

    int numLights = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    float radius = 0.0f;
    float currentDimming = 0.0f;
    ls::Vec2F position;
    const void* owner;

    const int numTiles = m_tiles.size();
    for (int i = numTiles - 1; i >= 0; --i)
    {
        if (currentDimming > dimmingCutoffThreshold) break;

        const Tile& tile = m_tiles[i].tile();
        if (std::optional<Light> lightOpt = tile.model().light(pos))
        {
            const Light& light = lightOpt.value();

            ++numLights;

            r += light.color().r;
            g += light.color().g;
            b += light.color().b;
            radius = std::max(radius, light.radius() * (1.0f - currentDimming));
            position = light.position();
            owner = light.owner();
        }

        currentDimming = std::max(currentDimming, tile.model().lightDimming());
    }

    if (numLights == 0) return std::nullopt;
    
    return Light(position, radius / numLights, sf::Color(r / numLights, g / numLights, b / numLights), owner);
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
