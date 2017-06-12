#include "entities/renderers/EntityRenderer.h"

EntityRenderer::EntityRenderer(Entity& owner) :
    EntityComponent(owner)
{

}
EntityRenderer::EntityRenderer(const EntityRenderer& other, Entity& owner) :
    EntityComponent(other, owner)
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

std::unique_ptr<EntityRenderer> EntityRenderer::clone(Entity& owner) const
{
    return std::make_unique<EntityRenderer>(*this, owner);
}
