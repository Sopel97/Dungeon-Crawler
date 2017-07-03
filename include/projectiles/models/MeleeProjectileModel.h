#pragma once

#include <Configuration.h>

#include <memory>

#include "AggroGroup.h"
#include "projectiles/models/ProjectileModel.h"

#include "colliders/ProjectileCollider.h"

#include "../LibS/Shapes.h"

#include "tiles/TileAttributeArray.h"

class Projectile;

class MeleeProjectileModel : public ProjectileModel
{
public:
    MeleeProjectileModel(Projectile& owner, ComponentCommonData* commonData);
    MeleeProjectileModel(const MeleeProjectileModel& other, Projectile& owner);
    ~MeleeProjectileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool canCollideWithTiles() const override;
    bool canCollideWithEntities() const override;
    ProjectileCollider collider() override;
    bool isDead() const override;
    AggroGroupId group() const override;
    int health() const override;
    void setHealth(int newHealth) override;

    const ls::Vec2F& position() const override;
    void setPosition(const ls::Vec2F& newPosition) override;

    const Entity* parentEntity() const override;

    void onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) override;
    void onCollidedWithEntity(EntityCollider& entityCollider) override;

    std::unique_ptr<ProjectileModel> clone(Projectile& owner) const override;

private:
    ls::Vec2F m_position;
    float m_radius;
    float m_offset;
    int m_health;
    AggroGroupId m_group;
    TileAttributeArray m_attributes;
};
