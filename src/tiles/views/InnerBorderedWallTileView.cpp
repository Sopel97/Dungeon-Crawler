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

    m_commonData->full = spriteSet + Vec2I(2*tileSize, 3*tileSize);
    m_commonData->top = spriteSet + Vec2I(2*tileSize, 1*tileSize);
    m_commonData->left = spriteSet + Vec2I(1*tileSize, 2*tileSize);
    m_commonData->topLeftConcave = spriteSet + Vec2I(1*tileSize, 1*tileSize);
    m_commonData->topLeftConvex = spriteSet + Vec2I(3*tileSize, 3*tileSize);

    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<int>();
}

void InnerBorderedWallTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
//NOTE: in current state it's just to present the idea and test things.
{
    int group = innerBorderGroup();

    bool isBottomTileGroupSame = map.at(x, y + 1, z).view().innerBorderGroup() == group;
    bool isRightTileGroupSame = map.at(x + 1, y, z).view().innerBorderGroup() == group;
    bool isBottomRightTileGroupSame = map.at(x, y + 1, z).view().innerBorderGroup() == group;

    const Vec2I* resultSprite = nullptr;

    resultSprite = &m_commonData->spriteSet;
    if(isBottomTileGroupSame && isRightTileGroupSame && isBottomRightTileGroupSame) resultSprite = &m_commonData->full;
    else if(!isRightTileGroupSame && isBottomTileGroupSame) resultSprite = &m_commonData->left;
    else if(isRightTileGroupSame && !isBottomTileGroupSame) resultSprite = &m_commonData->top;
    else if(!isRightTileGroupSame && !isBottomTileGroupSame) resultSprite = &m_commonData->topLeftConvex;
    else resultSprite = &m_commonData->topLeftConcave;

    if(resultSprite != nullptr)
    {
        sf::Sprite spr;
        spr.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
        spr.setTexture(texture());
        spr.setTextureRect(sf::IntRect(sf::Vector2i(resultSprite->x, resultSprite->y), sf::Vector2i(tileSize, tileSize)));
        renderTarget.draw(spr, renderStates);
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
