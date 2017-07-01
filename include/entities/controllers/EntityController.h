#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "Configuration.h"

#include <memory>

#include "entities/EntityComponent.h"

#include "../LibS/Geometry.h"

class Entity;
class World;

class EntityController : public EntityComponent<EntityController, Entity>
{
public:
    EntityController(Entity& owner);
    EntityController(const EntityController& other, Entity& owner);
    ~EntityController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    virtual void update(World& world, float dt);

    virtual void accelerate(const ls::Vec2F& dv);

    std::unique_ptr<EntityController> clone(Entity& owner) const override;
};

#endif // ENTITYCONTROLLER_H
