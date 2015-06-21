#include "OuterBorderedTileView.h"

#include "Root.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

using namespace Geo;

OuterBorderedTileView::OuterBorderedTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_currentSprite(0)
{

}
OuterBorderedTileView::OuterBorderedTileView(const OuterBorderedTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData)
{
    m_currentSprite = selectRandomSprite();
}
OuterBorderedTileView::~OuterBorderedTileView()
{
}

void OuterBorderedTileView::loadFromConfiguration(ConfigurationNode& config)
{
    std::string texturePath = config["texture"].get<std::string>();
    m_commonData->texture = ResourceManager::instance().get<sf::Texture>(texturePath);
    ConfigurationNode sprites = config["sprites"];
    int numberOfSprites = sprites.length();
    float weightSum = 0.0f;
    for(int i = 1; i <= numberOfSprites; ++i)
    {
        int x = sprites[i][1].get<int>();
        int y = sprites[i][2].get<int>();

        float weight = sprites[i][3].getDefault<float>(1.0f);
        m_commonData->sprites.emplace_back(x, y);
        weightSum += weight;
        m_commonData->spritesWeightsSums.emplace_back(weightSum);
    }
    m_commonData->borderSprites = Vec2I{config["borderSprites"][1], config["borderSprites"][2]};
}

void OuterBorderedTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(currentSprite().x, currentSprite().y), sf::Vector2i(tileSize, tileSize)));
    renderTarget.draw(spr, renderStates);
}
void OuterBorderedTileView::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
{

}

const sf::Texture& OuterBorderedTileView::texture() const
{
    return m_commonData->texture.get();
}
const Vec2I& OuterBorderedTileView::currentSprite() const
{
    return m_commonData->sprites[m_currentSprite];
}

int OuterBorderedTileView::selectRandomSprite() const
{
    const auto& spritesWeightsSums = m_commonData->spritesWeightsSums;

    if(spritesWeightsSums.size() == 0) return 0;

    float sumOfWeights = spritesWeightsSums.back();

    return std::lower_bound(spritesWeightsSums.begin(), spritesWeightsSums.end(), Root::instance().rng().nextFloat(0.0f, sumOfWeights)) - spritesWeightsSums.begin();
}

std::unique_ptr<TileView> OuterBorderedTileView::clone() const
{
    return std::make_unique<OuterBorderedTileView>(*this);
}
