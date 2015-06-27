#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "Configuration.h"

#include <memory>

class Entity;
class World;

class EntityController //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    EntityController(Entity* owner);
    EntityController(const EntityController& other);
    virtual ~EntityController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Entity* owner() const;

    void setOwner(Entity* newOwner);

    virtual void update(World* world, float dt); //NOTE: does not move the entity!!!

    virtual std::unique_ptr<EntityController> clone() const;
    virtual std::unique_ptr<EntityController> create(Entity* owner) const;
protected:
    Entity* m_owner;
};

#endif // ENTITYCONTROLLER_H
