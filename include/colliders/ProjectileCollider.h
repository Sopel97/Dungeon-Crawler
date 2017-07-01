#pragma once

#include "../LibS/Shapes.h"

class Projectile;

class ProjectileCollider
{
private:
    ls::Circle2F m_volume;
    Projectile* m_parent;

public:
    ProjectileCollider(Projectile& projectile, const ls::Circle2F& volume);

    const ls::Circle2F& volume() const;
    ls::Rectangle2F boundingBox() const;

    const Projectile& projectile() const;
    Projectile& projectile();
};