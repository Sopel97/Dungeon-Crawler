#include "tiles/renderers/InnerBorderedWallTileRenderer.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "TileLocation.h"

#include "SpriteBatch.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameConstants.h"

using namespace ls;

REGISTER_TILE_RENDERER_TYPE(InnerBorderedWallTileRenderer)

InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer(Tile& owner, CommonData& commonData) :
    TileRenderer(owner),
    m_commonData(&commonData)
{

}
InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other, Tile& owner) :
    TileRenderer(other, owner),
    m_commonData(other.m_commonData)
{

}
InnerBorderedWallTileRenderer::~InnerBorderedWallTileRenderer()
{

}

void InnerBorderedWallTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string textureName = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(textureName);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].get<bool>();

    Vec2I spriteSet{ config["spriteSet"][1].get<int>(), config["spriteSet"][2].get<int>() };

    m_commonData->spriteSet.full = spriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.top = spriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.left = spriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.topLeft = spriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.bottomRight = spriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);

    m_commonData->spriteSet.outerLeft = spriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.innerTopRight = spriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.innerBottomLeft = spriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.outerTopRight = spriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.outerBottomLeft = spriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.outerTop = spriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.outerTopLeft = spriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->spriteSet.innerBottomRight = spriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);

    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<std::string>();
}

void InnerBorderedWallTileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location) const
{
    if (!m_commonData->texture) return;
    draw(spriteBatch, location, ls::Vec2I(0, 0));
}

void InnerBorderedWallTileRenderer::drawMeta(SpriteBatch& spriteBatch, const TileLocation& location) const
{
    if (!m_commonData->texture || !m_commonData->hasMetaTexture) return;
    draw(spriteBatch, location, ls::Vec2I(texture().getSize().x / 2, 0));
}
void InnerBorderedWallTileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location, const ls::Vec2I& textureOffset) const
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

    int group = innerBorderGroup();
    int x = location.x;
    int y = location.y;
    int z = location.z;
    const MapLayer& map = *location.map;

    isGroupSame[TopLeft] = (map.at(x - 1, y - 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[Top] = (map.at(x + 0, y - 1, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[TopRight] = (map.at(x + 1, y - 1, z).tile().renderer().innerBorderGroup() == group);

    isGroupSame[Left] = (map.at(x - 1, y + 0, z).tile().renderer().innerBorderGroup() == group);
    isGroupSame[Center] = (map.at(x + 0, y + 0, z).tile().renderer().innerBorderGroup() == group);
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

        if (resultSprite != nullptr)
        {
            const ls::Vec2F sprite(*resultSprite);
            const ls::Vec2F size(GameConstants::tileSize, GameConstants::tileSize);
            const ls::Vec2F pos(x * size.x, y * size.y);

            spriteBatch.emplaceRectangle(&(texture()), pos, sprite + textureOffset, size);
        }
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

        if (resultSprite != nullptr)
        {
            const ls::Vec2F sprite(*resultSprite);
            const ls::Vec2F size(GameConstants::tileSize, GameConstants::tileSize);
            const ls::Vec2F pos((x-1) * size.x, y * size.y);

            spriteBatch.emplaceRectangle(&(texture()), pos, sprite + textureOffset, size);
        }
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

        if (resultSprite != nullptr)
        {
            const ls::Vec2F sprite(*resultSprite);
            const ls::Vec2F size(GameConstants::tileSize, GameConstants::tileSize);
            const ls::Vec2F pos(x * size.x, (y-1) * size.y);

            spriteBatch.emplaceRectangle(&(texture()), pos, sprite + textureOffset, size);
        }
    }

    {
        //on x-1, y-1

        const Vec2I* resultSprite = nullptr;

        if (!isGroupSame[TopLeft] && isGroupSame[Left] && isGroupSame[Top]) resultSprite = &spriteSet.outerTopLeft;
        else if (!isGroupSame[Top] && !isGroupSame[Left]) resultSprite = &spriteSet.innerBottomRight;

        if (resultSprite != nullptr)
        {
            const ls::Vec2F sprite(*resultSprite);
            const ls::Vec2F size(GameConstants::tileSize, GameConstants::tileSize);
            const ls::Vec2F pos((x-1) * size.x, (y-1) * size.y);

            spriteBatch.emplaceRectangle(&(texture()), pos, sprite + textureOffset, size);
        }
    }

}
const sf::Texture& InnerBorderedWallTileRenderer::texture() const
{
    return m_commonData->texture.get();
}
TileInnerBorderGroupType InnerBorderedWallTileRenderer::innerBorderGroup() const
{
    return m_commonData->innerBorderGroup;
}

bool InnerBorderedWallTileRenderer::isTall() const
{
    return true;
}

std::unique_ptr<TileRenderer> InnerBorderedWallTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<InnerBorderedWallTileRenderer>(*this, owner);
}
