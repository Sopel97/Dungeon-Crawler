#include "tiles/renderers/InnerBorderedWallTileRenderer.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "TileLocation.h"

#include "SpriteBatch.h"

#include "World.h"

#include "sprite/Spritesheet.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace ls;

REGISTER_TILE_RENDERER_TYPE(InnerBorderedWallTileRenderer)

InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer(Tile& owner, CommonData& commonData) :
    TileRenderer(owner),
    m_commonData(&commonData),
    m_spriteCache00(nullptr),
    m_spriteCache01(nullptr),
    m_spriteCache10(nullptr),
    m_spriteCache11(nullptr)
{

}
InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other, Tile& owner) :
    TileRenderer(other, owner),
    m_commonData(other.m_commonData),
    m_spriteCache00(other.m_spriteCache00),
    m_spriteCache01(other.m_spriteCache01),
    m_spriteCache10(other.m_spriteCache10),
    m_spriteCache11(other.m_spriteCache11)
{

}
InnerBorderedWallTileRenderer::~InnerBorderedWallTileRenderer()
{

}

void InnerBorderedWallTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string textureName = config["texture"].get<std::string>();
    m_commonData->spritesheet = ResourceManager<Spritesheet>::instance().get(textureName);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].get<bool>();

    Vec2I spriteSet{ config["spriteSet"][1].get<int>(), config["spriteSet"][2].get<int>() };

    m_commonData->spriteSet.full = spriteSet + Vec2I(2, 3);
    m_commonData->spriteSet.top = spriteSet + Vec2I(2, 1);
    m_commonData->spriteSet.left = spriteSet + Vec2I(1, 2);
    m_commonData->spriteSet.topLeft = spriteSet + Vec2I(1, 1);
    m_commonData->spriteSet.bottomRight = spriteSet + Vec2I(3, 3);

    m_commonData->spriteSet.outerLeft = spriteSet + Vec2I(0, 1);
    m_commonData->spriteSet.innerTopRight = spriteSet + Vec2I(3, 1);
    m_commonData->spriteSet.innerBottomLeft = spriteSet + Vec2I(1, 3);
    m_commonData->spriteSet.outerTopRight = spriteSet + Vec2I(3, 0);
    m_commonData->spriteSet.outerBottomLeft = spriteSet + Vec2I(0, 3);
    m_commonData->spriteSet.outerTop = spriteSet + Vec2I(1, 0);
    m_commonData->spriteSet.outerTopLeft = spriteSet + Vec2I(2, 2);
    m_commonData->spriteSet.innerBottomRight = spriteSet + Vec2I(0, 0);

    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<std::string>();
}

void InnerBorderedWallTileRenderer::drawGeometry(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch, SpriteBatch::SpriteGeometry& geometry) const
{
    const sf::Vector2f texOffset(texture().getSize().x / 2.0f, 0.0f);

    mainSpriteBatch.emplaceGeometry(&(texture()), geometry);

    if (m_commonData->hasMetaTexture)
    {
        for (auto& v : geometry.vertices)
        {
            v.texCoords += texOffset;
        }
        metaSpriteBatch.emplaceGeometry(&(texture()), geometry);
    }
}
void InnerBorderedWallTileRenderer::draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch, const TileLocation& location) const
{
    if (!m_commonData->spritesheet) return;

    const int x = location.x;
    const int y = location.y;

    //on x, y
    if (m_spriteCache00 != nullptr)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(*m_spriteCache00));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos(x * size.x, y * size.y);

        auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);
        drawGeometry(mainSpriteBatch, metaSpriteBatch, geometry);
    }

    //on x-1, y
    if (m_spriteCache10 != nullptr)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(*m_spriteCache10));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos((x - 1) * size.x, y * size.y);

        auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);
        drawGeometry(mainSpriteBatch, metaSpriteBatch, geometry);
    }

    //on x, y-1
    if (m_spriteCache01 != nullptr)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(*m_spriteCache01));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos(x * size.x, (y - 1) * size.y);

        auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);
        drawGeometry(mainSpriteBatch, metaSpriteBatch, geometry);
    }

    //on x-1, y-1
    if (m_spriteCache11 != nullptr)
    {
        const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(*m_spriteCache11));
        const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ 1, 1 }));
        const ls::Vec2F size(1.0f, 1.0f);
        const ls::Vec2F pos((x - 1) * size.x, (y - 1) * size.y);

        auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);
        drawGeometry(mainSpriteBatch, metaSpriteBatch, geometry);
    }
}
const sf::Texture& InnerBorderedWallTileRenderer::texture() const
{
    return m_commonData->spritesheet.get().texture();
}
const Spritesheet& InnerBorderedWallTileRenderer::spritesheet() const
{
    return m_commonData->spritesheet.get();
}
TileInnerBorderGroupType InnerBorderedWallTileRenderer::innerBorderGroup() const
{
    return m_commonData->innerBorderGroup;
}
void InnerBorderedWallTileRenderer::onTilePlaced(const TileLocation& location)
{
    updateCache(location);
}
void InnerBorderedWallTileRenderer::onTilePlacedNearby(const TileLocation& thisLocation, const ls::Vec2I& updatedTileOffset)
{
    updateCache(thisLocation);
}
void InnerBorderedWallTileRenderer::onTileRemovedNearby(const TileLocation& thisLocation, const ls::Vec2I& updatedTileOffset)
{
    updateCache(thisLocation);
}

void InnerBorderedWallTileRenderer::updateCache(const TileLocation& location)
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

    bool isGroupSame[9];

    const int group = innerBorderGroup();
    const int x = location.x;
    const int y = location.y;
    const int z = location.z;
    const MapLayer& map = *location.map;

    isGroupSame[TopLeft] = (map.at(x - 1, y - 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[Top] = (map.at(x + 0, y - 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[TopRight] = (map.at(x + 1, y - 1, z).tile().renderer().innerBorderGroup() == group);

    isGroupSame[Left] = (map.at(x - 1, y + 0, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[Center] = true;
    isGroupSame[Right] = (map.at(x + 1, y + 0, z).tile().renderer().innerBorderGroup() == group);

    isGroupSame[BottomLeft] = (map.at(x - 1, y + 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[Bottom] = (map.at(x + 0, y + 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[BottomRight] = (map.at(x + 1, y + 1, z).tile().renderer().innerBorderGroup() == group);

    CommonData::SpriteSet& spriteSet = m_commonData->spriteSet;

    {
        //on x, y
        const Vec2I* resultSprite = nullptr;

        if (isGroupSame[Bottom] && isGroupSame[Right] && isGroupSame[BottomRight]) resultSprite = &spriteSet.full;
        else if (!isGroupSame[Right] && isGroupSame[Bottom]) resultSprite = &spriteSet.left;
        else if (isGroupSame[Right] && !isGroupSame[Bottom]) resultSprite = &spriteSet.top;
        else if (!isGroupSame[Right] && !isGroupSame[Bottom]) resultSprite = &spriteSet.bottomRight;
        else resultSprite = &spriteSet.topLeft;

        m_spriteCache00 = resultSprite;
    }

    {
        //on x-1, y

        const Vec2I* resultSprite = nullptr;

        if (!isGroupSame[Left])
        {
            if (isGroupSame[Bottom] && !isGroupSame[Left]) resultSprite = &spriteSet.outerLeft;
            else resultSprite = &spriteSet.outerBottomLeft;
        }
        else if (!isGroupSame[BottomLeft] && isGroupSame[Bottom]) resultSprite = &spriteSet.innerTopRight;

        m_spriteCache10 = resultSprite;
    }

    {
        //on x, y-1

        const Vec2I* resultSprite = nullptr;

        if (!isGroupSame[Top])
        {
            if (isGroupSame[Right] && !isGroupSame[Top]) resultSprite = &spriteSet.outerTop;
            else resultSprite = &spriteSet.outerTopRight;
        }
        else if (!isGroupSame[TopRight] && isGroupSame[Right]) resultSprite = &spriteSet.innerBottomLeft;

        m_spriteCache01 = resultSprite;
    }

    {
        //on x-1, y-1

        const Vec2I* resultSprite = nullptr;

        if (!isGroupSame[TopLeft] && isGroupSame[Left] && isGroupSame[Top]) resultSprite = &spriteSet.outerTopLeft;
        else if (!isGroupSame[Top] && !isGroupSame[Left]) resultSprite = &spriteSet.innerBottomRight;

        m_spriteCache11 = resultSprite;
    }
}

std::unique_ptr<TileRenderer> InnerBorderedWallTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<InnerBorderedWallTileRenderer>(*this, owner);
}
