#pragma once

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "ResourceLoaders.h"
#include "ResourceManager.h"
#include "tiles/TileRandomizer.h"

class Entity;
class Player;

// TODO: common data storages for some entity components
class PlainEntityModel : public EntityModel
{
public:
    PlainEntityModel(Entity& owner, ComponentCommonData* commonData);
    PlainEntityModel(const PlainEntityModel& other, Entity& owner);
    ~PlainEntityModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool hasCollider() const override;
    float colliderRadius() const override;

    const ls::Vec2F& position() const override;
    void setPosition(const ls::Vec2F& newPosition) override;
    const ls::Vec2F& velocity() const override;
    void setVelocity(const ls::Vec2F& newVelocity) override;
    float distanceTravelled() const override;
    void setDistanceTravelled(float newDistanceTravelled) override;
    int health() const override;
    int maxHealth() const override;
    void setHealth(int newHealth) override;
    TileStack createCorpse() const override;
    EntityGroupId group() const override;

    ls::Vec2F displacementWhenMoved(float dt) override;

    float maxSpeed() const override;

    EntityModel::Direction directionOfMove() const override;
    void setDirectionOfMovement(EntityModel::Direction newDirection) override;

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;
protected:
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;
    int m_health;
    int m_maxHealth;
    EntityGroupId m_group;
    ResourceHandle<TilePrefab> m_corpseTile;
    TileRandomizer m_lootRandomizer;
};
