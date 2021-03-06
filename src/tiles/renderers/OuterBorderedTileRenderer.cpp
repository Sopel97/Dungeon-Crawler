#include "tiles/renderers/OuterBorderedTileRenderer.h"

#include "Root.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "MapLayer.h"

#include "TileLocation.h"

#include "SpriteBatch.h"

#include "World.h"

#include "sprite/Spritesheet.h"

#include "configuration/ShapeConfigurationReaders.h"
#include "configuration/ResourceConfigurationReaders.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

using namespace ls;

REGISTER_TILE_RENDERER_TYPE(OuterBorderedTileRenderer)

OuterBorderedTileRenderer::OuterBorderedTileRenderer(Tile& owner, CommonData& commonData) :
    TileRenderer(owner),
    m_commonData(&commonData),
    m_currentAnimatedSprite(nullptr)
{

}
OuterBorderedTileRenderer::OuterBorderedTileRenderer(const OuterBorderedTileRenderer& other, Tile& owner) :
    TileRenderer(other, owner),
    m_commonData(other.m_commonData),
    m_currentAnimatedSprite(other.m_currentAnimatedSprite)
{
}
OuterBorderedTileRenderer::~OuterBorderedTileRenderer()
{
}

void OuterBorderedTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationReaders::read(m_commonData->spritesheet, config["texture"]);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    ConfigurationReaders::read(m_commonData->borderSprites, config["borderSprites"]);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].get<int>();
}

void OuterBorderedTileRenderer::draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch, const TileLocation& location) const
{
    const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now()));
    const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
    const ls::Vec2F size(1.0f, 1.0f);
    const ls::Vec2F pos(location.x * size.x, location.y * size.y);

    mainSpriteBatch.emplaceRectangle(&(texture()), pos, size, sprite, spriteSize);
}

void OuterBorderedTileRenderer::drawOutside(SpriteBatch& spriteBatch, const TileLocation& location, const TileOuterBorderCache& cache) const
{
    const int x = location.x;
    const int y = location.y;

    const int sideBorderSpriteIndex = cache.sideSpriteIndex;

    Vec2I sideBorderSpritePosition = m_commonData->borderSprites + Vec2I {sideBorderSpriteIndex, 0};

    if(sideBorderSpriteIndex != -1)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(sideBorderSpritePosition));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos(x * size.x, y * size.y);

        spriteBatch.emplaceRectangle(&(texture()), pos, size, sprite, spriteSize);
    }


    const int cornerBorderSpriteIndex = cache.cornerSpriteIndex;

    Vec2I cornerBorderSpritePosition = m_commonData->borderSprites + Vec2I {cornerBorderSpriteIndex, 1};

    if(cornerBorderSpriteIndex != -1)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(cornerBorderSpritePosition));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos(x * size.x, y * size.y);

        spriteBatch.emplaceRectangle(&(texture()), pos, size, sprite, spriteSize);
    }
}

TileOuterBorderCache OuterBorderedTileRenderer::buildOuterBorderCache(const TileLocation& location) const
{
    enum Indices
    {
        TopLeft = 0,
        Top,
        TopRight,

        Left,
        Center,
        Right,

        BottomLeft,
        Bottom,
        BottomRight
    };

    bool isIdSame[9];

    const int id = m_owner->id();
    const int x = location.x;
    const int y = location.y;
    const int z = location.z;
    const MapLayer& map = *location.map;

    isIdSame[TopLeft] = (map.at(x - 1, y - 1, z).tile().id() == id);
    isIdSame[Top] = (map.at(x + 0, y - 1, z).tile().id() == id);
    isIdSame[TopRight] = (map.at(x + 1, y - 1, z).tile().id() == id);

    isIdSame[Left] = (map.at(x - 1, y + 0, z).tile().id() == id);
    isIdSame[Center] = true;
    isIdSame[Right] = (map.at(x + 1, y + 0, z).tile().id() == id);

    isIdSame[BottomLeft] = (map.at(x - 1, y + 1, z).tile().id() == id);
    isIdSame[Bottom] = (map.at(x + 0, y + 1, z).tile().id() == id);
    isIdSame[BottomRight] = (map.at(x + 1, y + 1, z).tile().id() == id);

    int sideBorderSpriteIndex = -1;

    if (isIdSame[Left]) sideBorderSpriteIndex += 1;
    if (isIdSame[Top]) sideBorderSpriteIndex += 2;
    if (isIdSame[Right]) sideBorderSpriteIndex += 4;
    if (isIdSame[Bottom]) sideBorderSpriteIndex += 8;

    int cornerBorderSpriteIndex = -1;

    if (isIdSame[TopLeft] && !isIdSame[Top] && !isIdSame[Left]) cornerBorderSpriteIndex += 1;
    if (isIdSame[TopRight] && !isIdSame[Top] && !isIdSame[Right]) cornerBorderSpriteIndex += 2;
    if (isIdSame[BottomRight] && !isIdSame[Bottom] && !isIdSame[Right]) cornerBorderSpriteIndex += 4;
    if (isIdSame[BottomLeft] && !isIdSame[Bottom] && !isIdSame[Left]) cornerBorderSpriteIndex += 8;

    return { sideBorderSpriteIndex, cornerBorderSpriteIndex };
}

const sf::Texture& OuterBorderedTileRenderer::texture() const
{
    return m_commonData->spritesheet.get().texture();
}
const Spritesheet& OuterBorderedTileRenderer::spritesheet() const
{
    return m_commonData->spritesheet.get();
}

int OuterBorderedTileRenderer::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}
bool OuterBorderedTileRenderer::hasOuterBorder() const
{
    return true;
}
bool OuterBorderedTileRenderer::coversOuterBorders() const
{
    return false;
}

void OuterBorderedTileRenderer::onTileInstantiated()
{
    m_currentAnimatedSprite = &(m_commonData->spriteSelector.onTileInstantiated());
}
void OuterBorderedTileRenderer::onTileQuantityChanged(int oldQuantity, int newQuantity)
{
    m_currentAnimatedSprite = &(m_commonData->spriteSelector.onTileQuantityChanged(newQuantity, *m_currentAnimatedSprite));
}

std::unique_ptr<TileRenderer> OuterBorderedTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<OuterBorderedTileRenderer>(*this, owner);
}
