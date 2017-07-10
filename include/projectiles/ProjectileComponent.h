#pragma once

#include <memory>

#include "Component.h"

#include "../LibS/Shapes.h"

class ComponentCommonData;
class Entity;
class World;
class TileStack;
class EntityCollider;
class TileCollider;

template <class ComponentType, class OwnerType>
class ProjectileComponent : public Component<ComponentType, OwnerType>
{
public:
    ProjectileComponent(OwnerType& owner) : Component<ComponentType, OwnerType>(owner) {};
    ProjectileComponent(const ProjectileComponent& other, OwnerType& owner) : Component<ComponentType, OwnerType>(other, owner) {};
    ~ProjectileComponent() override = default;

    virtual void onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) {}
    virtual void onProjectileCloned() {}
    virtual void onCollidedWithEntity(EntityCollider& entityCollider) {};
    virtual void onCollidedWithTile(TileCollider& tileCollider) {};
    virtual void onParentEntityDeleted() {};
};