#include "tiles/renderers/OuterBorderedTileRenderer.h"

#include "Root.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include "MapLayer.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

using namespace ls;

REGISTER_TILE_RENDERER_TYPE(OuterBorderedTileRenderer)

OuterBorderedTileRenderer::OuterBorderedTileRenderer(Tile& owner, ComponentCommonData* commonData) :
    TileRenderer(owner),
    m_commonData(static_cast<CommonData*>(commonData)),
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
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(textureName);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    m_commonData->borderSprites = Vec2I {config["borderSprites"][1].get<int>(), config["borderSprites"][2].get<int>()};
    m_commonData->outerBorderPriority = config["outerBorderPriority"].get<int>();
}

void OuterBorderedTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    const Vec2I& spritePos = m_currentAnimatedSprite->now();

    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x * GameConstants::tileSize), static_cast<float>(location.y * GameConstants::tileSize)));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(spritePos.x, spritePos.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

void OuterBorderedTileRenderer::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
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

const sf::Texture& OuterBorderedTileRenderer::texture() const
{
    return m_commonData->texture.get();
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

std::unique_ptr<ComponentCommonData> OuterBorderedTileRenderer::createCommonDataStorage()
{
    return std::make_unique<CommonData>();
}

std::unique_ptr<TileRenderer> OuterBorderedTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<OuterBorderedTileRenderer>(*this, owner);
}
