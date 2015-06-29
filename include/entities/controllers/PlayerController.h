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

    virtual std::unique_ptr<EntityController> clone() const;
    virtual std::unique_ptr<EntityController> create(Entity* owner) const;
protected:
    Player* m_playerOwner;
};

#endif // PLAYERCONTROLLER_H
