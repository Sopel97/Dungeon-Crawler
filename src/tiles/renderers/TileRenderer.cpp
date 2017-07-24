#include "tiles/renderers/TileRenderer.h"

#include "SFML/Graphics.hpp"
#include "TileLocation.h"
#include "GameConstants.h"

TileRenderer::TileRenderer(Tile& owner) :
    TileComponent(owner)
{

}
TileRenderer::TileRenderer(const TileRenderer& other, Tile& owner) :
    TileComponent(other, owner)
{

}
TileRenderer::~TileRenderer()
{

}

void TileRenderer::draw(SpriteBatch& spriteBatch, const TileLocation& location) const
{

}
void TileRenderer::drawOutside(SpriteBatch& spriteBatch, const TileLocation& location) const
{

}
void TileRenderer::drawMeta(SpriteBatch& spriteBatch, const TileLocation& location) const
{

}
void TileRenderer::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const InventorySlotView& slot) const
{

}

TileInnerBorderGroupType TileRenderer::innerBorderGroup() const
{
    return TileInnerBorderGroupType();
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

std::unique_ptr<TileRenderer> TileRenderer::clone(Tile& owner) const
{
    return std::make_unique<TileRenderer>(*this, owner);
}
