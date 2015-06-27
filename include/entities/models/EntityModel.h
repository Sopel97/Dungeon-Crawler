#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <Configuration.h>

#include <memory>

#include "../LibS/GeometryLight.h"

class Entity;

class EntityModel //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    EntityModel(Entity* owner);
    EntityModel(const EntityModel& other);
    virtual ~EntityModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Entity* owner() const;

    void setOwner(Entity* newOwner);

    virtual bool hasCollider() const;
    virtual float colliderRadius() const;

    virtual const Geo::Vec2F& position() const;
    virtual const Geo::Vec2F& velocity() const;

    virtual std::unique_ptr<EntityModel> clone() const;
    virtual std::unique_ptr<EntityModel> create(Entity* owner) const;
protected:
    Entity* m_owner;

private:
    static const Geo::Vec2F m_zeroVector;
};

#endif // ENTITYMODEL_H
