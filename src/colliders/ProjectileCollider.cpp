#include "colliders/ProjectileCollider.h"

ProjectileCollider::ProjectileCollider(Projectile& projectile, const ls::Circle2F& volume) :
    m_parent(&projectile),
    m_volume(volume)
{

}

const ls::Circle2F& ProjectileCollider::volume()
{
    return m_volume;
}
ls::Rectangle2F ProjectileCollider::boundingBox()
{
    return m_volume.boundingBox();
}