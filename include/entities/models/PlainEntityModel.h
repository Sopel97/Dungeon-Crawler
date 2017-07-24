#pragma once

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "ResourceLoaders.h"
#include "ResourceManager.h"
#include "tiles/TileRandomizer.h"

#include "AttributeArray.h"

class Entity;
class Player;

class PlainEntityModel : public EntityModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        int maxHealth;
        float maxSpeed;
        AggroGroupId group;
        ResourceHandle<TilePrefab> corpseTile;
        TileRandomizer lootRandomizer;
        AttributeArray attributes;
    };

    PlainEntityModel(Entity& owner, CommonData& commonData);
    PlainEntityModel(const PlainEntityModel& other, Entity& owner);
    ~PlainEntityModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    EntityCollider collider() override;

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
    AggroGroupId group() const override;
    const AttributeArray& attributes() const override;
    std::optional<Light> light() const override; // temp, for light testing

    float maxSpeed() const override;

    EntityModel::Direction directionOfMove() const override;
    void setDirectionOfMovement(EntityModel::Direction newDirection) override;

    void update(World& world, float dt) override;

    void accelerate(const ls::Vec2F& dv) override;

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;

protected:
    CommonData* const m_commonData;
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;
    int m_health;
};
