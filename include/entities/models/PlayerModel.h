#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"



class Entity;
class Player;

class PlayerModel : public EntityModel
{
public:
    PlayerModel(Entity* owner, Player* player);
    PlayerModel(const PlayerModel& other);
    virtual ~PlayerModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool hasCollider() const;
    virtual float colliderRadius() const;

    virtual const ls::Vec2F& position() const;
    virtual void setPosition(const ls::Vec2F& newPosition);
    virtual const ls::Vec2F& velocity() const;
    virtual void setVelocity(const ls::Vec2F& newVelocity);
    virtual float distanceTravelled() const;
    virtual void setDistanceTravelled(float newDistanceTravelled);

    virtual ls::Vec2F displacementWhenMoved(float dt);

    virtual float maxSpeed() const;

    virtual EntityModel::Direction directionOfMove() const;
    virtual void setDirectionOfMove(EntityModel::Direction newDirection);

    virtual std::unique_ptr<EntityModel> clone() const;
    virtual std::unique_ptr<EntityModel> create(Entity* owner) const;
protected:
    Player* m_playerOwner;
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;
};

#endif // PLAYERMODEL_H
