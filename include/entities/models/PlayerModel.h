#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/GeometryLight.h"

#include "../LibS/make_unique.h"

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

    virtual const Geo::Vec2F& position() const;
    virtual void setPosition(const Geo::Vec2F& newPosition);
    virtual const Geo::Vec2F& velocity() const;
    virtual void setVelocity(const Geo::Vec2F& newVelocity);

    virtual float maxSpeed() const;

    virtual EntityModel::Direction directionOfMove() const;
    virtual void setDirectionOfMove(EntityModel::Direction newDirection);

    virtual std::unique_ptr<EntityModel> clone() const;
    virtual std::unique_ptr<EntityModel> create(Entity* owner) const;
protected:
    Player* m_playerOwner;
    Geo::Vec2F m_position;
    Geo::Vec2F m_velocity;
    Direction m_directionOfMove;
};

#endif // PLAYERMODEL_H
