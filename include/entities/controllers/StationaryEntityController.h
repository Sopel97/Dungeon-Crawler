#pragma once

#include "EntityController.h"

#include "Configuration.h"

#include "ResourceLoaders.h"

#include <memory>

class Entity;
class World;
class Player;

class StationaryEntityController : public EntityController
{
public:
    StationaryEntityController(Entity& owner);
    StationaryEntityController(const StationaryEntityController& other, Entity& owner);
    ~StationaryEntityController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void update(World& world, float dt) override;

    void accelerate(const ls::Vec2F& dv) override;

    std::unique_ptr<EntityController> clone(Entity& owner) const override;
};
