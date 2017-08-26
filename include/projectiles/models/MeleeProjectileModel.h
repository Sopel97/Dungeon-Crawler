#pragma once

#include <Configuration.h>

#include <memory>

#include "AggroGroup.h"
#include "projectiles/models/ProjectileModel.h"

#include "colliders/ProjectileCollider.h"

#include "../LibS/Shapes.h"

#include "AttributeArray.h"

class Projectile;

class MeleeProjectileModel : public ProjectileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::vector<AttributeId> inheritedAttributes;
        int penetration;
    };

    MeleeProjectileModel(Projectile& owner, CommonData& commonData);
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

    void onParentEntityDeleted() override;

    void update(World& world, float dt) override;

    std::unique_ptr<ProjectileModel> clone(Projectile& owner) const override;

private:
    CommonData* const m_commonData;

    Entity* m_parentEntity;
    ls::Vec2F m_position;
    ls::Vec2F m_offset;
    float m_radius;
    int m_health;
    AggroGroupId m_group;
    AttributeArray m_attributes;
    std::vector<Entity*> m_hitEntities;
    float m_timeLeft;
};
