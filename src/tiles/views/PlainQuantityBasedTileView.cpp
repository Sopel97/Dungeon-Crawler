#include "PlainQuantityBasedTileView.h"

#include "Root.h"

#include "Tile.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

using namespace Geo;

PlainQuantityBasedTileView::PlainQuantityBasedTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_sprite(0, 0)
{
}
PlainQuantityBasedTileView::PlainQuantityBasedTileView(const PlainQuantityBasedTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData),
    m_sprite(other.m_sprite)
{
}
PlainQuantityBasedTileView::~PlainQuantityBasedTileView()
{

}

void PlainQuantityBasedTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);

    ConfigurationNode sprites = config["sprites"];
    m_commonData->spriteSet.loadFromConfiguration(sprites);

    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1); //-1 means that it will always be on top

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainQuantityBasedTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(location.x * GameConstants::tileSize, location.y * GameConstants::tileSize));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(m_sprite.x, m_sprite.y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlainQuantityBasedTileView::texture() const
{
    return m_commonData->texture.get();
}
bool PlainQuantityBasedTileView::coversOuterBorders() const
{
    return m_commonData->coversOuterBorders;
}
int PlainQuantityBasedTileView::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}

void PlainQuantityBasedTileView::onTilePlaced(const TileLocation& location)
{
    m_sprite = m_commonData->spriteSet.getSprite(m_owner->quantity());
}
void PlainQuantityBasedTileView::onTileQuantityChanged(int newQuantity)
{
    m_sprite = m_commonData->spriteSet.getSprite(newQuantity);
}

std::unique_ptr<TileView> PlainQuantityBasedTileView::clone() const
{
    return std::make_unique<PlainQuantityBasedTileView>(*this);
}
std::unique_ptr<TileView> PlainQuantityBasedTileView::create(Tile* owner) const
{
    return std::make_unique<PlainQuantityBasedTileView>(owner);
}
