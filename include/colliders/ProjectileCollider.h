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

    const ls::Circle2F& volume();
    ls::Rectangle2F boundingBox();
};