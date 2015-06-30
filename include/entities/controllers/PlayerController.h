#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "EntityController.h"

#include "Configuration.h"

#include <memory>

class Entity;
class World;
class Player;

class PlayerController : public EntityController
{
public:
    PlayerController(Entity* owner, Player* player);
    PlayerController(const PlayerController& other);
    virtual ~PlayerController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void update(World* world, float dt);

    virtual void move(const Geo::Vec2F& factor, float dt);
    virtual void accelerate(const Geo::Vec2F& dv);

    virtual std::unique_ptr<EntityController> clone() const;
    virtual std::unique_ptr<EntityController> create(Entity* owner) const;

protected:
    Player* m_playerOwner;
    bool m_acceleratedInLastFrame;
};

#endif // PLAYERCONTROLLER_H
