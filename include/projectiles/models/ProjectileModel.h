#pragma once

#include <Configuration.h>

#include <memory>

#include "projectiles/ProjectileComponent.h"

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
    virtual ls::Circle2F collider() const;
    virtual bool isDead() const;

    virtual const ls::Vec2F& position() const;
    virtual void setPosition(const ls::Vec2F& newPosition);

    virtual const Entity* parentEntity() const;

    std::unique_ptr<ProjectileModel> clone(Projectile& owner) const override;
};
