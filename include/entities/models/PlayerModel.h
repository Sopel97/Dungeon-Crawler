#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "EntityModel.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "ResourceLoaders.h"

#include "AttributeArray.h"

class Entity;
class Player;
class World;

class PlayerModel : public EntityModel
{
public:
    PlayerModel(Entity& owner);
    PlayerModel(Player& player, Entity& owner);
    PlayerModel(const PlayerModel& other, Entity& owner);
    ~PlayerModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    EntityCollider collider() override;

    const ls::Vec2F& position() const override;
    void setPosition(const ls::Vec2F& newPosition) override;
    const ls::Vec2F& velocity() const override;
    void setVelocity(const ls::Vec2F& newVelocity) override;
    float distanceTravelled() const override;
    AggroGroupId group() const override;
    TileStack& ammo() override;
    const AttributeArray& attributes() const override;
    std::optional<Light> light() const override;

    EntityModel::Direction directionOfMove() const override;

    void update(World& world, float dt) override;

    void accelerate(const ls::Vec2F& dv) override;

    std::unique_ptr<EntityModel> clone(Entity& owner) const override;
protected:
    static constexpr float m_maxSpeed = 64.0f / 32.0f;

    Player* m_playerOwner;
    ls::Vec2F m_position;
    ls::Vec2F m_velocity;
    Direction m_directionOfMove;
    float m_distanceTravelled;

    bool m_acceleratedHorizontallyInLastFrame;
    bool m_acceleratedVerticallyInLastFrame;

    void move(float dt);
};

#endif // PLAYERMODEL_H
