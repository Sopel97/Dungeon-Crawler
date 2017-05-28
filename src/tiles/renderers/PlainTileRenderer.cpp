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

PlainTileRenderer::PlainTileRenderer() :
    TileRenderer(),
    m_commonData(nullptr),
    m_spriteId(0)
{
}
PlainTileRenderer::PlainTileRenderer(const PlainTileRenderer& other) :
    TileRenderer(other),
    m_commonData(other.m_commonData),
    m_spriteId(other.m_spriteId)
{
}
PlainTileRenderer::~PlainTileRenderer()
{

}

void PlainTileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    std::string textureName = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(textureName);

    if (config["metaTexture"].exists())
    {
        std::string metaTextureName = config["metaTexture"].get<std::string>();
        m_commonData->metaTexture = ResourceManager::instance().get<sf::Texture>(metaTextureName);
    }
    else
    {
        m_commonData->metaTexture = nullptr;
    }

    ConfigurationNode sprites = config["sprites"];
    m_commonData->spriteSet.loadFromConfiguration(sprites);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->texture) return;
    draw(renderTarget, renderStates, location, m_commonData->texture.get());
}
void PlainTileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    if (!m_commonData->metaTexture) return;
    draw(renderTarget, renderStates, location, m_commonData->metaTexture.get());
}
void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const sf::Texture& texture) const
{
    const ls::Vec2I sprite = m_commonData->spriteSet.at(m_spriteId);

    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(static_cast<float>(location.x) * GameConstants::tileSize, static_cast<float>(location.y) * GameConstants::tileSize));
    spr.setTexture(texture);
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprite.x, sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}
void PlainTileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const
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
    m_spriteId = m_commonData->spriteSet.chooseRandomSprite();
}


std::unique_ptr<ComponentCommonData> PlainTileRenderer::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}
void PlainTileRenderer::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}

std::unique_ptr<TileRenderer> PlainTileRenderer::clone() const
{
    return std::make_unique<PlainTileRenderer>(*this);
}
