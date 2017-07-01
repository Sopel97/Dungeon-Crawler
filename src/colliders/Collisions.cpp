#include "colliders/Collisions.h"

#include "colliders/EntityCollider.h"
#include "colliders/TileCollider.h"
#include "colliders/ProjectileCollider.h"

bool Collisions::collide(const EntityCollider& lhs, EntityCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}

bool Collisions::collide(const EntityCollider& lhs, TileCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}
bool Collisions::collide(const TileCollider& lhs, EntityCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}

bool Collisions::collide(const ProjectileCollider& lhs, EntityCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}
bool Collisions::collide(const EntityCollider& lhs, ProjectileCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}

bool Collisions::collide(const ProjectileCollider& lhs, TileCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}
bool Collisions::collide(const TileCollider& lhs, ProjectileCollider& rhs)
{
    return ls::intersect(lhs.volume(), rhs.volume());
}