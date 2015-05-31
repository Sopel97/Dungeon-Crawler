#include "PlainTileView.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

PlainTileView::PlainTileView(Tile* owner) :
    TileView(owner)
{

}
PlainTileView::PlainTileView(const PlainTileView& other) :
    TileView(other),
    m_texture(other.m_texture),
    m_spritePosition(other.m_spritePosition)
{

}
PlainTileView::~PlainTileView()
{

}

void PlainTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_texture = ResourceManager::instance().get<sf::Texture>(texturePath);
    m_spritePosition.x = config["spritePosition"][1].get<int>();
    m_spritePosition.y = config["spritePosition"][2].get<int>();
}

void PlainTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map)
{

}

const ResourceHandle<sf::Texture> PlainTileView::texture()
{
    return m_texture;
}
const Geo::Vec2F PlainTileView::spritePosition()
{
    return m_spritePosition;
}

std::unique_ptr<TileView> PlainTileView::clone() const
{
    return std::make_unique<PlainTileView>(*this);
}
