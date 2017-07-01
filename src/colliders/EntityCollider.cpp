#include "colliders/EntityCollider.h"

EntityCollider::EntityCollider(Entity& entity, const ls::Circle2F& volume) :
    m_parent(&entity),
    m_volume(volume)
{

}

const ls::Circle2F& EntityCollider::volume() const
{
    return m_volume;
}
ls::Rectangle2F EntityCollider::boundingBox() const
{
    return m_volume.boundingBox();
}

const Entity& EntityCollider::entity() const
{
    return *m_parent;
}
Entity& EntityCollider::entity()
{
    return *m_parent;
}