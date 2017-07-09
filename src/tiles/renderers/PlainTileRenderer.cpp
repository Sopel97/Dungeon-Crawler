#include "tiles/renderers/PlainTileRenderer.h"

#include "Root.h"

#include "TileLocation.h"
#include "InventorySlotView.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include <memory>

#include "GameConstants.h"

using namespace ls;

REGISTER_TILE_RENDERER_TYPE(PlainTileRenderer)

PlainTileRenderer::PlainTileRenderer(Tile& owner, CommonData& commonData) :
    TileRenderer(owner),
    m_commonData(&commonData),
    m_currentAnimatedSprite(nullptr)
{
}
PlainTileRenderer::PlainTileRenderer(const PlainTileRenderer& other, Tile& owner) :
    TileRenderer(other, owner),
    m_commonData(other.m_commonData),
    m_currentAnimatedSprite(other.m_currentAnimatedSprite)
{
}
PlainTileRenderer::~PlainTileRenderer()
{

}

void PlainTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string textureName = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(textureName);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].getDefault<bool>(false);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->texture) return;
    draw(renderTarget, renderStates, location, Vec2I(0, 0));
}
void PlainTileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->texture || !m_commonData->hasMetaTexture) return;
    draw(renderTarget, renderStates, location, Vec2I(texture().getSize().x / 2, 0));
}
void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const ls::Vec2I& textureOffset) const
{
    const ls::Vec2I sprite = m_currentAnimatedSprite->now();

    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x) * GameConstants::tileSize, static_cast<float>(location.y) * GameConstants::tileSize));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x + textureOffset.x, sprite.y + textureOffset.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}
void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const
{
    const ls::Vec2I sprite = m_currentAnimatedSprite->now();
    const ls::Vec2I slotCenter = slot.center();
    const float x = slotCenter.x - GameConstants::tileSize / 2;
    const float y = slotCenter.y - GameConstants::tileSize / 2;
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x, y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x, sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlainTileRenderer::texture() const
{
    return m_commonData->texture.get();
}
bool PlainTileRenderer::coversOuterBorders() const
{
    return m_commonData->coversOuterBorders;
}
int PlainTileRenderer::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}

void PlainTileRenderer::onTileInstantiated()
{
    m_currentAnimatedSprite = &(m_commonData->spriteSelector.onTileInstantiated());
}
void PlainTileRenderer::onTileQuantityChanged(int oldQuantity, int newQuantity)
{
    m_currentAnimatedSprite = &(m_commonData->spriteSelector.onTileQuantityChanged(newQuantity, *m_currentAnimatedSprite));
}

std::unique_ptr<ComponentCommonData> PlainTileRenderer::createCommonDataStorage()
{
    return std::make_unique<CommonData>();
}

std::unique_ptr<TileRenderer> PlainTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<PlainTileRenderer>(*this, owner);
}
