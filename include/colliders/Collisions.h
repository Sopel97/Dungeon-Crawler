#pragma once

class EntityCollider;
class TileCollider;
class ProjectileCollider;

class Collisions
{
public:
    static bool collide(const EntityCollider& lhs, EntityCollider& rhs);

    static bool collide(const EntityCollider& lhs, TileCollider& rhs);
    static bool collide(const TileCollider& lhs, EntityCollider& rhs);

    static bool collide(const ProjectileCollider& lhs, EntityCollider& rhs);
    static bool collide(const EntityCollider& lhs, ProjectileCollider& rhs);

    static bool collide(const ProjectileCollider& lhs, TileCollider& rhs);
    static bool collide(const TileCollider& lhs, ProjectileCollider& rhs);
};