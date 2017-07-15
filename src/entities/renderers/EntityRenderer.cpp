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

void EntityRenderer::draw(SpriteBatch& spriteBatch) const
{

}
void EntityRenderer::drawMeta(SpriteBatch& spriteBatch) const
{

}

std::unique_ptr<EntityRenderer> EntityRenderer::clone(Entity& owner) const
{
    return std::make_unique<EntityRenderer>(*this, owner);
}
