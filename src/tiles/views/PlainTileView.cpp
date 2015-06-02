#include "PlainTileView.h"

#include "Root.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

PlainTileView::PlainTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_currentSprite(0)
{
}
PlainTileView::PlainTileView(const PlainTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData)
{
    if(m_commonData->sprites.size() == 0) m_currentSprite = 0;
    else m_currentSprite = Root::instance().rng().nextInt32(0, m_commonData->sprites.size() - 1);
}
PlainTileView::~PlainTileView()
{

}

void PlainTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);
    ConfigurationNode sprites = config["sprites"];
    int numberOfSprites = sprites.length();
    for(int i = 1; i <= numberOfSprites; ++i)
    {
        int x = sprites[i][1].get<int>();
        int y = sprites[i][2].get<int>();
        m_commonData->sprites.emplace_back(x, y);
    }
}

void PlainTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map)
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x * 32.0f, y * 32.0f));
    spr.setTexture(m_commonData->texture.get());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(currentSprite().x, currentSprite().y), sf::Vector2i(32, 32)));
    renderTarget.draw(spr, renderStates);
}

const ResourceHandle<sf::Texture> PlainTileView::texture()
{
    return m_commonData->texture;
}
const Geo::Vec2I& PlainTileView::currentSprite()
{
    return m_commonData->sprites[m_currentSprite];
}

std::unique_ptr<TileView> PlainTileView::clone() const
{
    return std::make_unique<PlainTileView>(*this);
}
