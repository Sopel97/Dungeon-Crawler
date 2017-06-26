#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <Configuration.h>

#include <memory>

#include "entities/EntityComponent.h"

#include "../LibS/Geometry.h"

class Entity;

class EntityModel : public EntityComponent<EntityModel, Entity>
{
public:
    enum Direction //ordered as in sprites
    {
        North = 0,
        West = 1,
        East = 2,
        South = 3
    };

    EntityModel(Entity& owner);
    EntityModel(const EntityModel& other, Entity& owner);
    ~EntityModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    virtual bool hasCollider() const;
    virtual float colliderRadius() const;

    virtual const ls::Vec2F& position() const;
    virtual void setPosition(const ls::Vec2F& newPosition);
    virtual const ls::Vec2F& velocity() const;
    virtual void setVelocity(const ls::Vec2F& newVelocity);
    virtual float distanceTravelled() const;
    virtual void setDistanceTravelled(float newDistanceTravelled);
    virtual int health() const;
    virtual int maxHealth() const;

    virtual ls::Vec2F displacementWhenMoved(float dt);

    virtual float maxSpeed() const;

    virtual Direction directionOfMove() const;
    virtual void setDirectionOfMove(Direction newDirection);

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;
protected:

private:
    static const ls::Vec2F m_someVector;
};

#endif // ENTITYMODEL_H
