#include "tiles/renderers/PlainQuantityBasedTileRenderer.h"

#include "Root.h"

#include "tiles/Tile.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

using namespace ls;

PlainQuantityBasedTileRenderer::PlainQuantityBasedTileRenderer() :
    TileRenderer(),
    m_commonData(nullptr),
    m_spriteId(0)
{
}
PlainQuantityBasedTileRenderer::PlainQuantityBasedTileRenderer(const PlainQuantityBasedTileRenderer& other) :
    TileRenderer(other),
    m_commonData(other.m_commonData),
    m_spriteId(other.m_spriteId)
{
}
PlainQuantityBasedTileRenderer::~PlainQuantityBasedTileRenderer()
{

}

void PlainQuantityBasedTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);

    ConfigurationNode sprites = config["sprites"];
    m_commonData->spriteSet.loadFromConfiguration(sprites);

    ConfigurationNode metaSprites = config["metaSprites"];
    int numberOfMetaSprites = metaSprites.length();
    for (int i = 1; i <= numberOfMetaSprites; ++i)
    {
        ConfigurationNode sprite = metaSprites[i];
        m_commonData->respectiveMetaSprites.emplace_back(
            sprite[1].get<int>(),
            sprite[2].get<int>()
            );
    }

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainQuantityBasedTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    const Vec2I& spritePos = m_commonData->spriteSet.at(m_spriteId);
    draw(renderTarget, renderStates, location, spritePos);
}
void PlainQuantityBasedTileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (m_commonData->respectiveMetaSprites.size() <= m_spriteId) return;

    const Vec2I& spritePos = m_commonData->respectiveMetaSprites[m_spriteId];
    draw(renderTarget, renderStates, location, spritePos);
}
void PlainQuantityBasedTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const ls::Vec2I& sprite) const
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x) * GameConstants::tileSize, static_cast<float>(location.y) * GameConstants::tileSize));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x, sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}
void PlainQuantityBasedTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const
{
    const Vec2I& spritePos = m_commonData->spriteSet.at(m_spriteId);
    const ls::Vec2I slotCenter = slot.center();
    const float x = slotCenter.x - GameConstants::tileSize / 2;
    const float y = slotCenter.y - GameConstants::tileSize / 2;
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x, y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(spritePos.x, spritePos.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlainQuantityBasedTileRenderer::texture() const
{
    return m_commonData->texture.get();
}
bool PlainQuantityBasedTileRenderer::coversOuterBorders() const
{
    return m_commonData->coversOuterBorders;
}
int PlainQuantityBasedTileRenderer::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}

void PlainQuantityBasedTileRenderer::onTileInstantiated()
{
    m_spriteId = m_commonData->spriteSet.getSprite(1);
}
void PlainQuantityBasedTileRenderer::onTileQuantityChanged(int oldQuantity, int newQuantity)
{
    m_spriteId = m_commonData->spriteSet.getSprite(newQuantity);
}

std::unique_ptr<ComponentCommonData> PlainQuantityBasedTileRenderer::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}
void PlainQuantityBasedTileRenderer::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}

std::unique_ptr<TileRenderer> PlainQuantityBasedTileRenderer::clone() const
{
    return std::make_unique<PlainQuantityBasedTileRenderer>(*this);
}
