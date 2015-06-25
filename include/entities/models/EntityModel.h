#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <Configuration.h>

#include <memory>

class Entity;

class EntityModel
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

    virtual std::unique_ptr<EntityModel> clone() const;
    virtual std::unique_ptr<EntityModel> create(Entity* owner) const;
protected:
    Entity* m_owner;
};

#endif // ENTITYMODEL_H
