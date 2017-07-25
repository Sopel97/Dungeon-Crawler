#include "tiles/renderers/OuterBorderedTileRenderer.h"

#include "Root.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "MapLayer.h"

#include "TileLocation.h"

#include "SpriteBatch.h"

#include "sprite/Spritesheet.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

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
    std::string textureName = config["texture"].get<std::string>();
    m_commonData->spritesheet = ResourceManager<Spritesheet>::instance().get(textureName);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    m_commonData->borderSprites = Vec2I {config["borderSprites"][1].get<int>(), config["borderSprites"][2].get<int>()};
    m_commonData->outerBorderPriority = config["outerBorderPriority"].get<int>();
}

void OuterBorderedTileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location) const
{
    const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now()));
    const ls::Vec2F size(static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize));
    const ls::Vec2F pos(location.x * size.x, location.y * size.y);

    spriteBatch.emplaceRectangle(&(texture()), pos, sprite, size);
}

void OuterBorderedTileRenderer::drawOutside(SpriteBatch& spriteBatch, const TileLocation& location) const
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

    int id = m_owner->id();
    int x = location.x;
    int y = location.y;
    int z = location.z;
    const MapLayer& map = *location.map;

    isIdSame[TopLeft]     = (map.at(x - 1, y - 1, z).tile().id() == id);
    isIdSame[Top]         = (map.at(x + 0, y - 1, z).tile().id() == id);
    isIdSame[TopRight]    = (map.at(x + 1, y - 1, z).tile().id() == id);

    isIdSame[Left]        = (map.at(x - 1, y + 0, z).tile().id() == id);
    isIdSame[Center]      = (map.at(x + 0, y + 0, z).tile().id() == id);
    isIdSame[Right]       = (map.at(x + 1, y + 0, z).tile().id() == id);

    isIdSame[BottomLeft]  = (map.at(x - 1, y + 1, z).tile().id() == id);
    isIdSame[Bottom]      = (map.at(x + 0, y + 1, z).tile().id() == id);
    isIdSame[BottomRight] = (map.at(x + 1, y + 1, z).tile().id() == id);

    int sideBorderSpriteIndex = -1;

    if(isIdSame[Left]) sideBorderSpriteIndex += 1;
    if(isIdSame[Top]) sideBorderSpriteIndex += 2;
    if(isIdSame[Right]) sideBorderSpriteIndex += 4;
    if(isIdSame[Bottom]) sideBorderSpriteIndex += 8;


    Vec2I sideBorderSpritePosition = m_commonData->borderSprites + Vec2I {sideBorderSpriteIndex, 0};

    if(sideBorderSpriteIndex != -1)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(sideBorderSpritePosition));
        const ls::Vec2F size(static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize));
        const ls::Vec2F pos(x * size.x, y * size.y);

        spriteBatch.emplaceRectangle(&(texture()), pos, sprite, size);
    }


    int cornerBorderSpriteIndex = -1;

    if(isIdSame[TopLeft] && !isIdSame[Top] && !isIdSame[Left]) cornerBorderSpriteIndex += 1;
    if(isIdSame[TopRight] && !isIdSame[Top] && !isIdSame[Right]) cornerBorderSpriteIndex += 2;
    if(isIdSame[BottomRight] && !isIdSame[Bottom] && !isIdSame[Right]) cornerBorderSpriteIndex += 4;
    if(isIdSame[BottomLeft] && !isIdSame[Bottom] && !isIdSame[Left]) cornerBorderSpriteIndex += 8;

    Vec2I cornerBorderSpritePosition = m_commonData->borderSprites + Vec2I {cornerBorderSpriteIndex, 1};

    if(cornerBorderSpriteIndex != -1)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(cornerBorderSpritePosition));
        const ls::Vec2F size(static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize));
        const ls::Vec2F pos(x * size.x, y * size.y);

        spriteBatch.emplaceRectangle(&(texture()), pos, sprite, size);
    }
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
