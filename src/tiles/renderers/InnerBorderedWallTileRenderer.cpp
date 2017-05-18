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

InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer() :
    TileRenderer(),
    m_commonData(nullptr)

{

}
InnerBorderedWallTileRenderer::InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other) :
    TileRenderer(other),
    m_commonData(other.m_commonData)
{

}
InnerBorderedWallTileRenderer::~InnerBorderedWallTileRenderer()
{

}

void InnerBorderedWallTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);

    Vec2I mainSpriteSet{ config["mainSpriteSet"][1].get<int>(), config["mainSpriteSet"][2].get<int>() };
    Vec2I metaSpriteSet{ config["metaSpriteSet"][1].get<int>(), config["metaSpriteSet"][2].get<int>() };

    //main sprites
    m_commonData->mainSprites.full = mainSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.top = mainSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.left = mainSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.topLeft = mainSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.bottomRight = mainSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);

    m_commonData->mainSprites.outerLeft = mainSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.innerTopRight = mainSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.innerBottomLeft = mainSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.outerTopRight = mainSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.outerBottomLeft = mainSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.outerTop = mainSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.outerTopLeft = mainSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->mainSprites.innerBottomRight = mainSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);

    //meta sprites
    m_commonData->metaSprites.full = metaSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.top = metaSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.left = metaSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.topLeft = metaSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.bottomRight = metaSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);

    m_commonData->metaSprites.outerLeft = metaSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.innerTopRight = metaSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 1 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.innerBottomLeft = metaSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.outerTopRight = metaSpriteSet + Vec2I(3 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.outerBottomLeft = metaSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 3 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.outerTop = metaSpriteSet + Vec2I(1 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.outerTopLeft = metaSpriteSet + Vec2I(2 * GameConstants::tileFullSpriteSize, 2 * GameConstants::tileFullSpriteSize);
    m_commonData->metaSprites.innerBottomRight = metaSpriteSet + Vec2I(0 * GameConstants::tileFullSpriteSize, 0 * GameConstants::tileFullSpriteSize);

    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<int>();
}

void InnerBorderedWallTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    draw(renderTarget, renderStates, location, m_commonData->mainSprites);
}

void InnerBorderedWallTileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    draw(renderTarget, renderStates, location, m_commonData->metaSprites);
}
void InnerBorderedWallTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const CommonData::SpriteSet& spriteSet) const
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
            spr.setTexture(texture());
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
            spr.setTexture(texture());
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
            spr.setTexture(texture());
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
            spr.setTexture(texture());
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

}
const sf::Texture& InnerBorderedWallTileRenderer::texture() const
{
    return m_commonData->texture.get();
}
int InnerBorderedWallTileRenderer::innerBorderGroup() const
{
    return m_commonData->innerBorderGroup;
}

bool InnerBorderedWallTileRenderer::isTall() const
{
    return true;
}

std::unique_ptr<ComponentCommonData> InnerBorderedWallTileRenderer::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}

void InnerBorderedWallTileRenderer::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}
std::unique_ptr<TileRenderer> InnerBorderedWallTileRenderer::clone() const
{
    return std::make_unique<InnerBorderedWallTileRenderer>(*this);
}
