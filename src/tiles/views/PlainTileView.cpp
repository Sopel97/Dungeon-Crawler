#include "PlainTileView.h"

#include "Root.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

using namespace Geo;

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
    m_currentSprite = selectRandomSprite();
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

        float weight = sprites[i][3].getDefault<float>(1.0f);
        m_commonData->sprites.emplace_back(x, y);
        float lastSum = 0.0f;
        if(i > 1) lastSum = m_commonData->spritesWeightsSums.back();
        m_commonData->spritesWeightsSums.emplace_back(weight + lastSum);
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

int PlainTileView::selectRandomSprite() const
{
    const auto& spritesWeightsSums = m_commonData->spritesWeightsSums;

    if(spritesWeightsSums.size() == 0) return 0;

    float sumOfWeights = spritesWeightsSums.back();

    return std::lower_bound(spritesWeightsSums.begin(), spritesWeightsSums.end(), Root::instance().rng().nextFloat(0.0f, sumOfWeights)) - spritesWeightsSums.begin();
}

std::unique_ptr<TileView> PlainTileView::clone() const
{
    return std::make_unique<PlainTileView>(*this);
}
