#include "colliders/EntityCollider.h"

EntityCollider::EntityCollider(Entity& entity, const ls::Circle2F& volume) :
    m_parent(&entity),
    m_volume(volume)
{

}

const ls::Circle2F& EntityCollider::volume()
{
    return m_volume;
}
ls::Rectangle2F EntityCollider::boundingBox()
{
    return m_volume.boundingBox();
}