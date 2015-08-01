#include "PlainTileView.h"

#include "Root.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>

#include "GameConstants.h"

using namespace Geo;

PlainTileView::PlainTileView(Tile* owner) :
    TileView(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_selectedSprite(0)
{
}
PlainTileView::PlainTileView(const PlainTileView& other) :
    TileView(other),
    m_commonData(other.m_commonData)
{
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
    m_commonData->outerBorderPriority = config["outerBorderPriority"].getDefault<int>(-1);

    bool defaultForBorderCovering = m_commonData->outerBorderPriority == -1 ? true : false;
    m_commonData->coversOuterBorders = config["coversOuterBorders"].getDefault<bool>(defaultForBorderCovering);
}

void PlainTileView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(location.x * GameConstants::tileSize, location.y * GameConstants::tileSize));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(selectedSprite().x, selectedSprite().y), sf::Vector2i(GameConstants::tileSize, GameConstants::tileSize)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlainTileView::texture() const
{
    return m_commonData->texture.get();
}
const Geo::Vec2I& PlainTileView::selectedSprite() const
{
    return m_commonData->sprites[m_selectedSprite];
}
bool PlainTileView::coversOuterBorders() const
{
    return m_commonData->coversOuterBorders;
}
int PlainTileView::outerBorderPriority() const
{
    return m_commonData->outerBorderPriority;
}

int PlainTileView::selectRandomSprite() const
{
    const auto& spritesWeightsSums = m_commonData->spritesWeightsSums;

    if(spritesWeightsSums.size() == 0) return 0;

    float sumOfWeights = spritesWeightsSums.back();

    return std::lower_bound(spritesWeightsSums.begin(), spritesWeightsSums.end(), Root::instance().rng().nextFloat(0.0f, sumOfWeights)) - spritesWeightsSums.begin();
}

void PlainTileView::onTilePlaced(const TileLocation& location)
{
    m_selectedSprite = selectRandomSprite();
}
void PlainTileView::onTileRemoved(const TileLocation& location)
{

}

std::unique_ptr<TileView> PlainTileView::clone() const
{
    return std::make_unique<PlainTileView>(*this);
}
std::unique_ptr<TileView> PlainTileView::create(Tile* owner) const
{
    return std::make_unique<PlainTileView>(owner);
}
