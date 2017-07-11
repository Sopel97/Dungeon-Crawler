#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <Configuration.h>

#include <memory>
#include <optional>

#include "entities/EntityComponent.h"
#include "AggroGroup.h"

#include "Light.h"

#include "tiles/TileStack.h"

#include "colliders/EntityCollider.h"

#include "../LibS/Geometry.h"

class Entity;
class AttributeArray;

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

    virtual EntityCollider collider();

    virtual const ls::Vec2F& position() const;
    virtual void setPosition(const ls::Vec2F& newPosition);
    virtual const ls::Vec2F& velocity() const;
    virtual void setVelocity(const ls::Vec2F& newVelocity);
    virtual float distanceTravelled() const;
    virtual void setDistanceTravelled(float newDistanceTravelled);
    virtual int health() const;
    virtual int maxHealth() const;
    virtual void setHealth(int newHealth);
    virtual TileStack createCorpse() const;
    virtual AggroGroupId group() const;
    virtual TileStack& ammo();
    virtual const AttributeArray& attributes() const;
    virtual std::optional<Light> light() const;

    void onEntityInstantiated(const ls::Vec2F& pos) override;

    virtual float maxSpeed() const;

    virtual Direction directionOfMove() const;
    virtual void setDirectionOfMovement(Direction newDirection);

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;
protected:

private:
    static const ls::Vec2F m_someVector;
};

#endif // ENTITYMODEL_H
