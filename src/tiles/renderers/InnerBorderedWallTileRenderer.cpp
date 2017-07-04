#include "tiles/renderers/InnerBorderedWallTileRenderer.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "TileLocation.h"

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

    if (config["metaTexture"].exists())
    {
        std::string metaTextureName = config["metaTexture"].get<std::string>();
        m_commonData->metaTexture = ResourceManager::instance().get<sf::Texture>(metaTextureName);
    }
    else
    {
        m_commonData->metaTexture = nullptr;
    }

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

void InnerBorderedWallTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->texture) return;
    draw(renderTarget, renderStates, location, m_commonData->texture.get());
}

void InnerBorderedWallTileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->metaTexture) return;
    draw(renderTarget, renderStates, location, m_commonData->metaTexture.get());
}
void InnerBorderedWallTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const sf::Texture& texture) const
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
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(static_cast<float>(x * GameConstants::tileSize), static_cast<float>(y * GameConstants::tileSize)));
            spr.setTexture(texture);
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
            renderTarget.draw(spr, renderStates);
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
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(static_cast<float>((x - 1) * GameConstants::tileSize), static_cast<float>(y * GameConstants::tileSize)));
            spr.setTexture(texture);
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
            renderTarget.draw(spr, renderStates);
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
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(static_cast<float>(x * GameConstants::tileSize), static_cast<float>((y - 1) * GameConstants::tileSize)));
            spr.setTexture(texture);
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

    {
        //on x-1, y-1

        const Vec2I* resultSprite = nullptr;

        if (!isGroupSame[TopLeft] && isGroupSame[Left] && isGroupSame[Top]) resultSprite = &spriteSet.outerTopLeft;
        else if (!isGroupSame[Top] && !isGroupSame[Left]) resultSprite = &spriteSet.innerBottomRight;

        if (resultSprite != nullptr)
        {
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(static_cast<float>((x - 1) * GameConstants::tileSize), static_cast<float>((y - 1) * GameConstants::tileSize)));
            spr.setTexture(texture);
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
            renderTarget.draw(spr, renderStates);
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

std::unique_ptr<ComponentCommonData> InnerBorderedWallTileRenderer::createCommonDataStorage()
{
    return std::make_unique<CommonData>();
}

std::unique_ptr<TileRenderer> InnerBorderedWallTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<InnerBorderedWallTileRenderer>(*this, owner);
}
