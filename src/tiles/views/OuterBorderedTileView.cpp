#include "tiles/views/OuterBorderedTileView.h"

#include "Root.h"

#include "tiles/Tile.h"

#include "MapLayer.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

using namespace ls;

OuterBorderedTileView::OuterBorderedTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_sprite(0, 0)
{

}
OuterBorderedTileView::OuterBorderedTileView(const OuterBorderedTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData),
    m_sprite(other.m_sprite)
{
}
OuterBorderedTileView::~OuterBorderedTileView()
{
}

void OuterBorderedTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);
    ConfigurationNode sprites = config["sprites"];
    m_commonData->spriteSet.loadFromConfiguration(sprites);

    m_commonData->borderSprites = Vec2I {config["borderSprites"][1].get<int>(), config["borderSprites"][2].get<int>()};
    m_commonData->outerBorderPriority = config["outerBorderPriority"].get<int>();

    m_sprite = m_commonData->spriteSet.defaultSprite();
}

void OuterBorderedTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x * GameConstants::tileSize), static_cast<float>(location.y * GameConstants::tileSize)));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(m_sprite.x, m_sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

void OuterBorderedTileView::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
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
    const MapLayer& map = location.map;

    isIdSame[TopLeft]     = (map.at(x - 1, y - 1, z).id() == id);
    isIdSame[Top]         = (map.at(x + 0, y - 1, z).id() == id);
    isIdSame[TopRight]    = (map.at(x + 1, y - 1, z).id() == id);

    isIdSame[Left]        = (map.at(x - 1, y + 0, z).id() == id);
    isIdSame[Center]      = (map.at(x + 0, y + 0, z).id() == id);
    isIdSame[Right]       = (map.at(x + 1, y + 0, z).id() == id);

    isIdSame[BottomLeft]  = (map.at(x - 1, y + 1, z).id() == id);
    isIdSame[Bottom]      = (map.at(x + 0, y + 1, z).id() == id);
    isIdSame[BottomRight] = (map.at(x + 1, y + 1, z).id() == id);

    int sideBorderSpriteIndex = -1;

    if(isIdSame[Left]) sideBorderSpriteIndex += 1;
    if(isIdSame[Top]) sideBorderSpriteIndex += 2;
    if(isIdSame[Right]) sideBorderSpriteIndex += 4;
    if(isIdSame[Bottom]) sideBorderSpriteIndex += 8;


    Vec2I sideBorderSpritePosition = m_commonData->borderSprites + Vec2I {GameConstants::tileFullSpriteSize * sideBorderSpriteIndex, 0};

    if(sideBorderSpriteIndex != -1)
    {
        sf::Sprite convexBorderSprite;
        convexBorderSprite.setPosition(sf::Vector2f(static_cast<float>(x * GameConstants::tileSize), static_cast<float>(y * GameConstants::tileSize)));
        convexBorderSprite.setTexture(texture());
        convexBorderSprite.setTextureRect(sf::IntRect(sf::Vector2i(sideBorderSpritePosition.x, sideBorderSpritePosition.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
        renderTarget.draw(convexBorderSprite, renderStates);
    }


    int cornerBorderSpriteIndex = -1;

    if(isIdSame[TopLeft] && !isIdSame[Top] && !isIdSame[Left]) cornerBorderSpriteIndex += 1;
    if(isIdSame[TopRight] && !isIdSame[Top] && !isIdSame[Right]) cornerBorderSpriteIndex += 2;
    if(isIdSame[BottomRight] && !isIdSame[Bottom] && !isIdSame[Right]) cornerBorderSpriteIndex += 4;
    if(isIdSame[BottomLeft] && !isIdSame[Bottom] && !isIdSame[Left]) cornerBorderSpriteIndex += 8;

    Vec2I cornerBorderSpritePosition = m_commonData->borderSprites + Vec2I {GameConstants::tileFullSpriteSize * cornerBorderSpriteIndex, GameConstants::tileFullSpriteSize};

    if(cornerBorderSpriteIndex != -1)
    {
        sf::Sprite concaveBorderSprite;
        concaveBorderSprite.setPosition(sf::Vector2f(static_cast<float>(x) * static_cast<float>(GameConstants::tileSize), static_cast<float>(y) * static_cast<float>(GameConstants::tileSize)));
        concaveBorderSprite.setTexture(texture());
        concaveBorderSprite.setTextureRect(sf::IntRect(sf::Vector2i(cornerBorderSpritePosition.x, cornerBorderSpritePosition.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
        renderTarget.draw(concaveBorderSprite, renderStates);
    }
}

const sf::Texture& OuterBorderedTileView::texture() const
{
    return m_commonData->texture.get();
}

int OuterBorderedTileView::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}
bool OuterBorderedTileView::hasOuterBorder() const
{
    return true;
}
bool OuterBorderedTileView::coversOuterBorders() const
{
    return false;
}

void OuterBorderedTileView::onTilePlaced(const TileLocation& location)
{
    m_sprite = m_commonData->spriteSet.chooseRandomSprite();
}

std::unique_ptr<TileView> OuterBorderedTileView::clone() const
{
    return std::make_unique<OuterBorderedTileView>(*this);
}
std::unique_ptr<TileView> OuterBorderedTileView::create(Tile* owner) const
{
    return std::make_unique<OuterBorderedTileView>(owner);
}
