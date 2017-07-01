#include "colliders/ProjectileCollider.h"

ProjectileCollider::ProjectileCollider(Projectile& projectile, const ls::Circle2F& volume) :
    m_parent(&projectile),
    m_volume(volume)
{

}

const ls::Circle2F& ProjectileCollider::volume() const
{
    return m_volume;
}
ls::Rectangle2F ProjectileCollider::boundingBox() const
{
    return m_volume.boundingBox();
}
const Projectile& ProjectileCollider::projectile() const
{
    return *m_parent;
}
Projectile& ProjectileCollider::projectile()
{
    return *m_parent;
}