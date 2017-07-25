#include "tiles/renderers/PlainTileRenderer.h"

#include "Root.h"

#include "TileLocation.h"
#include "InventorySlotView.h"

#include "SpriteBatch.h"

#include "sprite/Spritesheet.h"

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
    m_commonData->spritesheet = ResourceManager::instance().get<Spritesheet>(textureName);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].getDefault<bool>(false);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainTileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location) const
{
    if (!m_commonData->spritesheet) return;
    draw(spriteBatch, location, Vec2I(0, 0));
}
void PlainTileRenderer::drawMeta(SpriteBatch& spriteBatch, const TileLocation& location) const
{
    if (!m_commonData->spritesheet || !m_commonData->hasMetaTexture) return;
    draw(spriteBatch, location, Vec2I(texture().getSize().x / 2, 0));
}
void PlainTileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location, const ls::Vec2I& textureOffset) const
{
    const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now()));
    const ls::Vec2F size(static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize));
    const ls::Vec2F pos(location.x * size.x, location.y * size.y);

    spriteBatch.emplaceRectangle(&(texture()), pos, sprite + textureOffset, size);
}
void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const InventorySlotView& slot) const
{
    const ls::Vec2I sprite = spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now());
    const ls::Vec2I slotCenter = slot.center();
    const float x = slotCenter.x - GameConstants::tileSize / 2.0f;
    const float y = slotCenter.y - GameConstants::tileSize / 2.0f;
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x, y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x, sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlainTileRenderer::texture() const
{
    return m_commonData->spritesheet.get().texture();
}
const Spritesheet& PlainTileRenderer::spritesheet() const
{
    return m_commonData->spritesheet.get();
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

std::unique_ptr<TileRenderer> PlainTileRenderer::clone(Tile& owner) const
{
    return std::make_unique<PlainTileRenderer>(*this, owner);
}
