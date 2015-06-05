#include "InnerBorderedWallTileView.h"

#include "Tile.h"
#include "TileStack.h"
#include "MapLayer.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace Geo;

InnerBorderedWallTileView::InnerBorderedWallTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>())

{

}
InnerBorderedWallTileView::InnerBorderedWallTileView(const InnerBorderedWallTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData)
{

}
InnerBorderedWallTileView::~InnerBorderedWallTileView()
{

}

void InnerBorderedWallTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);

    Vec2I spriteSet {config["spriteSet"][1].get<int>(), config["spriteSet"][2].get<int>()};
    m_commonData->spriteSet = spriteSet;

    m_commonData->full = spriteSet + Vec2I(2 * tileSize, 3 * tileSize);
    m_commonData->top = spriteSet + Vec2I(2 * tileSize, 1 * tileSize);
    m_commonData->left = spriteSet + Vec2I(1 * tileSize, 2 * tileSize);
    m_commonData->topLeft = spriteSet + Vec2I(1 * tileSize, 1 * tileSize);
    m_commonData->bottomRight = spriteSet + Vec2I(3 * tileSize, 3 * tileSize);

    m_commonData->outerLeft = spriteSet + Vec2I(0 * tileSize, 1 * tileSize);
    m_commonData->innerTopRight = spriteSet + Vec2I(3 * tileSize, 1 * tileSize);
    m_commonData->innerBottomLeft = spriteSet + Vec2I(1 * tileSize, 3 * tileSize);
    m_commonData->outerTopRight = spriteSet + Vec2I(3 * tileSize, 0 * tileSize);
    m_commonData->outerBottomLeft = spriteSet + Vec2I(0 * tileSize, 3 * tileSize);
    m_commonData->outerTop = spriteSet + Vec2I(1 * tileSize, 0 * tileSize);
    m_commonData->outerTopLeft = spriteSet + Vec2I(2 * tileSize, 2 * tileSize);
    m_commonData->innerBottomRight = spriteSet + Vec2I(0 * tileSize, 0 * tileSize);

    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<int>();
}

void InnerBorderedWallTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
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

    isGroupSame[TopLeft]     = (map.at(x - 1, y - 1, z).view().innerBorderGroup() == group);
    isGroupSame[Top]         = (map.at(x + 0, y - 1, z).view().innerBorderGroup() == group);
    isGroupSame[TopRight]    = (map.at(x + 1, y - 1, z).view().innerBorderGroup() == group);

    isGroupSame[Left]        = (map.at(x - 1, y + 0, z).view().innerBorderGroup() == group);
    isGroupSame[Center]      = (map.at(x + 0, y + 0, z).view().innerBorderGroup() == group);
    isGroupSame[Right]       = (map.at(x + 1, y + 0, z).view().innerBorderGroup() == group);

    isGroupSame[BottomLeft]  = (map.at(x - 1, y + 1, z).view().innerBorderGroup() == group);
    isGroupSame[Bottom]      = (map.at(x + 0, y + 1, z).view().innerBorderGroup() == group);
    isGroupSame[BottomRight] = (map.at(x + 1, y + 1, z).view().innerBorderGroup() == group);

    {
        //on x, y
        const Vec2I* resultSprite = nullptr;

        if(isGroupSame[Bottom] && isGroupSame[Right] && isGroupSame[BottomRight]) resultSprite = &m_commonData->full;
        else if(!isGroupSame[Right] && isGroupSame[Bottom]) resultSprite = &m_commonData->left;
        else if(isGroupSame[Right] && !isGroupSame[Bottom]) resultSprite = &m_commonData->top;
        else if(!isGroupSame[Right] && !isGroupSame[Bottom]) resultSprite = &m_commonData->bottomRight;
        else resultSprite = &m_commonData->topLeft;

        if(resultSprite != nullptr)
        {
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
            spr.setTexture(texture());
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(tileSize, tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

    {
        //on x-1, y

        const Vec2I* resultSprite = nullptr;

        if(!isGroupSame[Left])
        {
            if(isGroupSame[Bottom] && !isGroupSame[Left]) resultSprite = &m_commonData->outerLeft;
            else resultSprite = &m_commonData->outerBottomLeft;
        }
        else if(!isGroupSame[BottomLeft] && isGroupSame[Bottom]) resultSprite = &m_commonData->innerTopRight;

        if(resultSprite != nullptr)
        {
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f((x - 1) * tileSize, y * tileSize));
            spr.setTexture(texture());
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(tileSize, tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

    {
        //on x, y-1

        const Vec2I* resultSprite = nullptr;

        if(!isGroupSame[Top])
        {
            if(isGroupSame[Right] && !isGroupSame[Top]) resultSprite = &m_commonData->outerTop;
            else resultSprite = &m_commonData->outerTopRight;
        }
        else if(!isGroupSame[TopRight] && isGroupSame[Right]) resultSprite = &m_commonData->innerBottomLeft;

        if(resultSprite != nullptr)
        {
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f(x * tileSize, (y - 1) * tileSize));
            spr.setTexture(texture());
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(tileSize, tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

    {
        //on x-1, y-1

        const Vec2I* resultSprite = nullptr;

        if(!isGroupSame[TopLeft] && isGroupSame[Left] && isGroupSame[Top]) resultSprite = &m_commonData->outerTopLeft;
        else if(!isGroupSame[Top] && !isGroupSame[Left]) resultSprite = &m_commonData->innerBottomRight;

        if(resultSprite != nullptr)
        {
            sf::Sprite spr;
            spr.setPosition(sf::Vector2f((x - 1) * tileSize, (y - 1) * tileSize));
            spr.setTexture(texture());
            spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(tileSize, tileSize)));
            renderTarget.draw(spr, renderStates);
        }
    }

}

const sf::Texture& InnerBorderedWallTileView::texture() const
{
    return m_commonData->texture.get();
}
const Geo::Vec2I& InnerBorderedWallTileView::spriteSet() const
{
    return m_commonData->spriteSet;
}
int InnerBorderedWallTileView::innerBorderGroup() const
{
    return m_commonData->innerBorderGroup;
}

std::unique_ptr<TileView> InnerBorderedWallTileView::clone() const
{
    return std::make_unique<InnerBorderedWallTileView>(*this);
}
