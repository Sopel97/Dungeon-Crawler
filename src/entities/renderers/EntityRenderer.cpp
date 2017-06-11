#include "entities/renderers/EntityRenderer.h"

EntityRenderer::EntityRenderer() :
    EntityComponent()
{

}
EntityRenderer::EntityRenderer(const EntityRenderer& other) :
    EntityComponent(other)
{

}
EntityRenderer::~EntityRenderer()
{

}

void EntityRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}
void EntityRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}

std::unique_ptr<EntityRenderer> EntityRenderer::clone() const
{
    return std::make_unique<EntityRenderer>(*this);
}
