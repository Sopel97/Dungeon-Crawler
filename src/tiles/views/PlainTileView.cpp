#include "PlainTileView.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

PlainTileView::PlainTileView(Tile* owner) :
    TileView(owner)
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

void PlainTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, MapLayer& map)
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
