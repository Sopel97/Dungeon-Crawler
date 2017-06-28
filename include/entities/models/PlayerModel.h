#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "ResourceLoaders.h"

class Entity;
class Player;

class PlayerModel : public EntityModel
{
public:
    PlayerModel(Entity& owner, ComponentCommonData* commonData);
    PlayerModel(Player& player, Entity& owner);
    PlayerModel(const PlayerModel& other, Entity& owner);
    ~PlayerModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool hasCollider() const override;
    float colliderRadius() const override;

    const ls::Vec2F& position() const override;
    void setPosition(const ls::Vec2F& newPosition) override;
    const ls::Vec2F& velocity() const override;
    void setVelocity(const ls::Vec2F& newVelocity) override;
    float distanceTravelled() const override;
    void setDistanceTravelled(float newDistanceTravelled) override;
    EntityGroupId group() const override;

    ls::Vec2F displacementWhenMoved(float dt) override;

    float maxSpeed() const override;

    EntityModel::Direction directionOfMove() const override;
    void setDirectionOfMovement(EntityModel::Direction newDirection) override;

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;
protected:
    Player* m_playerOwner;
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;
};

#endif // PLAYERMODEL_H
