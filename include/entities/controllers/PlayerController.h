#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "EntityController.h"

#include "Configuration.h"

#include "ResourceLoaders.h"

#include <memory>

class Entity;
class World;
class Player;

class PlayerController : public EntityController
{
public:
    PlayerController();
    PlayerController(Player* player);
    PlayerController(const PlayerController& other);
    ~PlayerController() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void update(World& world, float dt) override;

    void move(const ls::Vec2F& factor, float dt) override;
    void accelerate(const ls::Vec2F& dv) override;

    std::unique_ptr<EntityController> clone() const override;

protected:
    Player* m_playerOwner;
    bool m_acceleratedHorizontallyInLastFrame;
    bool m_acceleratedVerticallyInLastFrame;
};

REGISTER_ENTITY_CONTROLLER_TYPE(PlayerController)

#endif // PLAYERCONTROLLER_H
