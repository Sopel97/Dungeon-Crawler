#include "entities/views/EntityView.h"

EntityView::EntityView(Entity* owner) :
    m_owner(owner)
{

}
EntityView::EntityView(const EntityView& other) :
    m_owner(other.m_owner)
{

}
EntityView::~EntityView()
{

}

void EntityView::loadFromConfiguration(ConfigurationNode& config)
{

}

void EntityView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}
void EntityView::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{

}

const Entity* EntityView::owner() const
{
    return m_owner;
}

void EntityView::setOwner(Entity* newOwner)
{
    m_owner = newOwner;
}

std::unique_ptr<EntityView> EntityView::clone() const
{
    return std::make_unique<EntityView>(*this);
}
std::unique_ptr<EntityView> EntityView::create(Entity* owner) const
{
    return std::make_unique<EntityView>(owner);
}
