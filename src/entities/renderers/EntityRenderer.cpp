#include "entities/renderers/EntityRenderer.h"

EntityRenderer::EntityRenderer(Entity* owner) :
    m_owner(owner)
{

}
EntityRenderer::EntityRenderer(const EntityRenderer& other) :
    m_owner(other.m_owner)
{

}
EntityRenderer::~EntityRenderer()
{

}

void EntityRenderer::loadFromConfiguration(ConfigurationNode& config)
{

}

void EntityRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}
void EntityRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}

const Entity* EntityRenderer::owner() const
{
    return m_owner;
}

void EntityRenderer::setOwner(Entity* newOwner)
{
    m_owner = newOwner;
}

std::unique_ptr<EntityRenderer> EntityRenderer::clone() const
{
    return std::make_unique<EntityRenderer>(*this);
}
std::unique_ptr<EntityRenderer> EntityRenderer::create(Entity* owner) const
{
    return std::make_unique<EntityRenderer>(owner);
}
