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
    int spriteSetX = config["spriteSet"][1].get<int>();
    int spriteSetY = config["spriteSet"][2].get<int>();
    m_commonData->spriteSet = Vec2I(spriteSetX, spriteSetY);
    m_commonData->innerBorderGroup = config["innerBorderGroup"].get<int>();
}

void InnerBorderedWallTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map)
//NOTE: in current state it's just to present the idea and test things.
{
    int group = innerBorderGroup();
    //only some of them interest us. For not it will remain ugly. Later it will be changed along with better interfaces for map and tile stack.
    const TileStack& bottomNeighbourStack = map.at(x, y + 1);
    const TileStack& rightNeighbourStack = map.at(x + 1, y);
    const TileStack& bottomRightNeighbourStack = map.at(x, y + 1);

    const Tile* bottomNeighbourTile = bottomNeighbourStack.tiles().size()>=2 ? bottomNeighbourStack.tiles()[1] : nullptr;
    const Tile* rightNeighbourTile = rightNeighbourStack.tiles().size()>=2 ? rightNeighbourStack.tiles()[1] : nullptr;
    const Tile* bottomRightNeighbourTile = bottomRightNeighbourStack.tiles().size()>=2 ? bottomRightNeighbourStack.tiles()[1] : nullptr;

    bool isBottomTileGroupSame = bottomNeighbourTile == nullptr ? false : bottomNeighbourTile->view()->innerBorderGroup() == group;
    bool isRightTileGroupSame = rightNeighbourTile == nullptr ? false : rightNeighbourTile->view()->innerBorderGroup() == group;
    bool isBottomRightTileGroupSame = bottomRightNeighbourTile == nullptr ? false : bottomRightNeighbourTile->view()->innerBorderGroup() == group;

    const Vec2I& spriteSet = m_commonData->spriteSet;
    const Vec2I full = spriteSet + Vec2I(64, 96);
    const Vec2I top = spriteSet + Vec2I(64, 32);
    const Vec2I left = spriteSet + Vec2I(32, 64);
    const Vec2I topLeftConcave = spriteSet + Vec2I(32, 32);
    const Vec2I topLeftConvex = spriteSet + Vec2I(96, 96);

    const Vec2I* destinationSprite = nullptr;

    destinationSprite = &spriteSet;
    if(isBottomTileGroupSame && isRightTileGroupSame && isBottomRightTileGroupSame) destinationSprite = &full;
    else if(!isRightTileGroupSame && isBottomTileGroupSame) destinationSprite = &left;
    else if(isRightTileGroupSame && !isBottomTileGroupSame) destinationSprite = &top;
    else if(!isRightTileGroupSame && !isBottomTileGroupSame) destinationSprite = &topLeftConvex;
    else destinationSprite = &topLeftConcave;


    if(destinationSprite == nullptr) return;

    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x * 32.0f, y * 32.0f));
    spr.setTexture(m_commonData->texture.get());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(destinationSprite->x, destinationSprite->y), sf::Vector2i(32, 32)));
    renderTarget.draw(spr, renderStates);
}

const ResourceHandle<sf::Texture>& InnerBorderedWallTileView::texture() const
{
    return m_commonData->texture;
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
