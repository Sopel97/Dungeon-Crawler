#pragma once

#include "../LibS/Shapes.h"

class Entity;

class EntityCollider
{
private:
    ls::Circle2F m_volume;
    Entity* m_parent;

public:
    EntityCollider(Entity& entity, const ls::Circle2F& volume);

    const ls::Circle2F& volume() const;
    ls::Rectangle2F boundingBox() const;

    const Entity& entity() const;
    Entity& entity();
};