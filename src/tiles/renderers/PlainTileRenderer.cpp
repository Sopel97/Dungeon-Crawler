#include "tiles/renderers/PlainTileRenderer.h"

#include "Root.h"

#include "TileLocation.h"
#include "InventorySlotView.h"

#include "SpriteBatch.h"

#include "World.h"

#include "sprite/Spritesheet.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include <memory>

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
    m_commonData->spritesheet = ResourceManager<Spritesheet>::instance().get(textureName);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].getDefault<bool>(false);
    m_commonData->spriteSize = config["spriteSize"].getDefault<int>(1);

    m_commonData->spriteSelector.loadFromConfiguration(config);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainTileRenderer::draw(SpriteBatch& mainSriteBatch, SpriteBatch& metaSpriteBatch, const TileLocation& location) const
{
    if (!m_commonData->spritesheet) return;

    const ls::Vec2F sprite = static_cast<ls::Vec2F>(spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now()));
    const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(spritesheet().gridSizeToTexSize({ m_commonData->spriteSize, m_commonData->spriteSize }));
    const ls::Vec2F size(static_cast<float>(m_commonData->spriteSize), static_cast<float>(m_commonData->spriteSize));
    const ls::Vec2F pos(static_cast<float>(location.x - (m_commonData->spriteSize - 1)), static_cast<float>(location.y - (m_commonData->spriteSize - 1)));

    auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);
    mainSriteBatch.emplaceGeometry(&(texture()), geometry);

    if (!m_commonData->hasMetaTexture) return;
    const sf::Vector2f texOffset(texture().getSize().x / 2.0f, 0.0f);
    for (auto& v : geometry.vertices)
    {
        v.texCoords += texOffset;
    }
    metaSpriteBatch.emplaceGeometry(&(texture()), geometry);
}

void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const InventorySlotView& slot) const
{
    const ls::Vec2I sprite = spritesheet().gridCoordsToTexCoords(m_currentAnimatedSprite->now());
    const ls::Vec2I slotCenter = slot.center();
    const float x = slotCenter.x - InventorySlotView::tileSize / 2.0f;
    const float y = slotCenter.y - InventorySlotView::tileSize / 2.0f;
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x, y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x, sprite.y), sf::Vector2i(InventorySlotView::tileSize, InventorySlotView::tileSize)));
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
