#include "tiles/renderers/TileRenderer.h"

#include "SFML/Graphics.hpp"
#include "TileLocation.h"
#include "GameConstants.h"

TileRenderer::TileRenderer() :
    m_owner(nullptr)
{

}
TileRenderer::TileRenderer(const TileRenderer& other) :
    m_owner(other.m_owner)
{

}
TileRenderer::~TileRenderer()
{

}

void TileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{

}
void TileRenderer::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{

}
void TileRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{

}

void TileRenderer::loadFromConfiguration(ConfigurationNode& config)
{

}

void TileRenderer::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}

int TileRenderer::innerBorderGroup() const
{
    return -1;
}

int TileRenderer::outerBorderPriority() const
{
    return -1;
}
bool TileRenderer::hasOuterBorder() const
{
    return false;
}
bool TileRenderer::coversOuterBorders() const
{
    return true;
}

bool TileRenderer::isTall() const
{
    return false;
}

void TileRenderer::onTilePlaced(const TileLocation& location)
{

}
void TileRenderer::onTileRemoved(const TileLocation& location)
{

}
void TileRenderer::onTileQuantityChanged(int oldQuantity, int newQuantity)
{

}
void TileRenderer::onTileInstantiated()
{

}

std::unique_ptr<ComponentCommonData> TileRenderer::createCommonDataStorage() const
{
    return nullptr;
}
void TileRenderer::setCommonDataStorage(ComponentCommonData& commonData)
{

}

std::unique_ptr<TileRenderer> TileRenderer::clone() const
{
    return std::make_unique<TileRenderer>(*this);
}
