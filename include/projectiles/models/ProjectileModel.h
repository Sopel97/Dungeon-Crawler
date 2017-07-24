#pragma once

#include <Configuration.h>

#include <memory>

#include "AggroGroup.h"
#include "projectiles/ProjectileComponent.h"

#include "colliders/ProjectileCollider.h"

#include "../LibS/Shapes.h"

class Projectile;

class ProjectileModel : public ProjectileComponent<ProjectileModel, Projectile>
{
public:
    ProjectileModel(Projectile& owner);
    ProjectileModel(const ProjectileModel& other, Projectile& owner);
    ~ProjectileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    virtual bool canCollideWithTiles() const;
    virtual bool canCollideWithEntities() const;
    virtual ProjectileCollider collider();
    virtual bool isDead() const;
    virtual AggroGroupId group() const;
    virtual int health() const;
    virtual void setHealth(int newHealth);

    virtual const ls::Vec2F& position() const;
    virtual void setPosition(const ls::Vec2F& newPosition);

    virtual const Entity* parentEntity() const;

    virtual void update(World& world, float dt);

    std::unique_ptr<ProjectileModel> clone(Projectile& owner) const override;
};
