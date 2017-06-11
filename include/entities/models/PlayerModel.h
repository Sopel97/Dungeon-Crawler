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
    PlayerModel();
    PlayerModel(Player* player);
    PlayerModel(const PlayerModel& other);
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

    ls::Vec2F displacementWhenMoved(float dt) override;

    float maxSpeed() const override;

    EntityModel::Direction directionOfMove() const override;
    void setDirectionOfMove(EntityModel::Direction newDirection) override;

    std::unique_ptr<EntityModel> clone() const override;
protected:
    Player* m_playerOwner;
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;
};

REGISTER_ENTITY_MODEL_TYPE(PlayerModel)

#endif // PLAYERMODEL_H
