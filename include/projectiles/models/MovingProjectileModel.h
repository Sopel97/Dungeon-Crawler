#pragma once

#include <Configuration.h>

#include <memory>

#include "AggroGroup.h"
#include "projectiles/models/ProjectileModel.h"

#include "colliders/ProjectileCollider.h"

#include "OscillatingLightSource.h"

#include "../LibS/Shapes.h"

#include "AttributeArray.h"

class Projectile;

class MovingProjectileModel : public ProjectileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::vector<AttributeId> inheritedAttributes;
        std::optional<OscillatingLightSource> light;
        float initialSpeed;
        float radius;
        float acceleration;
        float maxTravelDistance;
        float maxTravelTime;
        int penetration;
    };

    MovingProjectileModel(Projectile& owner, CommonData& commonData);
    MovingProjectileModel(const MovingProjectileModel& other, Projectile& owner);
    ~MovingProjectileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool canCollideWithTiles() const override;
    bool canCollideWithEntities() const override;
    ProjectileCollider collider() override;
    bool isDead() const override;
    AggroGroupId group() const override;
    int health() const override;
    void setHealth(int newHealth) override;
    std::optional<Light> light() const override;

    const ls::Vec2F& position() const override;
    void setPosition(const ls::Vec2F& newPosition) override;
    ls::Vec2F velocity() const override;

    const Entity* parentEntity() const override;

    void onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) override;
    void onCollidedWithEntity(EntityCollider& entityCollider) override;
    void onCollidedWithTile(TileCollider& tileCollider) override;

    void update(World& world, float dt) override;

    std::unique_ptr<ProjectileModel> clone(Projectile& owner) const override;

private:
    CommonData* const m_commonData;

    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    AggroGroupId m_group;
    int m_health;
    float m_distanceTravelled;
    float m_timeTravelled;
    std::vector<Entity*> m_hitEntities;
    AttributeArray m_attributes;
};
