#include "TallEntityDrawable.h"

#include "Entity.h"
#include "EntityModel.h"
#include "EntityView.h"

using namespace Geo;

TallEntityDrawable::TallEntityDrawable(Entity* entity) :
    m_entity(entity)
{
    float radius = m_entity->model().colliderRadius();
    Vec2F halfSize {radius / 2.0f, radius / 2.0f};
    m_center = m_entity->model().position();
    m_boundingRectangle = RectangleF(m_center - halfSize, m_center + halfSize);
}

TallEntityDrawable::~TallEntityDrawable()
{

}

bool TallEntityDrawable::isEntity() const
{
    return true;
}
const RectangleF& TallEntityDrawable::boundingRectangle() const
{
    return m_boundingRectangle;
}
const Vec2F& TallEntityDrawable::center() const
{
    return m_center;
}

void TallEntityDrawable::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_entity->view().draw(renderTarget, renderStates);
}
