#include "tiles/renderers/TileRenderer.h"

#include "SFML/Graphics.hpp"
#include "TileLocation.h"
#include "GameConstants.h"

TileRenderer::TileRenderer() :
    TileComponent()
{

}
TileRenderer::TileRenderer(const TileRenderer& other) :
    TileComponent(other)
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
void TileRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const
{

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

std::unique_ptr<TileRenderer> TileRenderer::clone() const
{
    return std::make_unique<TileRenderer>(*this);
}
