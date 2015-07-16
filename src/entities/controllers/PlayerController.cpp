#include "PlayerController.h"

#include "World.h"

#include "Entity.h"
#include "EntityModel.h"

#include "../LibS/make_unique.h"

#include "../LibS/GeometryLight.h"

#include <cmath>

using namespace Geo;

PlayerController::PlayerController(Entity* owner, Player* player) :
    EntityController(owner),
    m_playerOwner(player),
    m_acceleratedInLastFrame(false)
{

}
PlayerController::PlayerController(const PlayerController& other) :
    EntityController(other.m_owner),
    m_playerOwner(other.m_playerOwner),
    m_acceleratedInLastFrame(other.m_acceleratedInLastFrame)
{

}
PlayerController::~PlayerController()
{

}

void PlayerController::loadFromConfiguration(ConfigurationNode& config)
{

}

void PlayerController::update(World* world, float dt)
{
    auto& model = m_owner->model();
    Vec2F velocity = model.velocity();
    const Vec2F& position = model.position();
    float minSpeed = model.minSpeed();
    float maxSpeed = model.maxSpeed();

    float speed = velocity.magnitude();
    if(speed > maxSpeed)
    {
        velocity *= maxSpeed/speed;
    }
    if(!m_acceleratedInLastFrame)
    {
        velocity *= std::pow(1.0f-world->drag(position), dt);
        if(velocity.magnitude() < minSpeed) velocity = Vec2F(0.0f, 0.0f);
    }

    if(velocity.x < -0.01f && std::abs(velocity.x) > std::abs(velocity.y)) model.setDirectionOfMove(EntityModel::Direction::West);
    if(velocity.x > 0.01f && std::abs(velocity.x) > std::abs(velocity.y))  model.setDirectionOfMove(EntityModel::Direction::East);
    if(velocity.y < -0.01f && std::abs(velocity.y) > std::abs(velocity.x)) model.setDirectionOfMove(EntityModel::Direction::North);
    if(velocity.y > 0.01f && std::abs(velocity.y) > std::abs(velocity.x))  model.setDirectionOfMove(EntityModel::Direction::South);

    model.setVelocity(velocity);

    m_acceleratedInLastFrame = false;
}

void PlayerController::move(const Geo::Vec2F& factor, float dt)
{
    auto& model = m_owner->model();
    Vec2F position = model.position();
    position += model.velocity()*factor*dt;
    model.setPosition(position);
}
void PlayerController::accelerate(const Geo::Vec2F& dv)
{
    auto& model = m_owner->model();
    Vec2F velocity = model.velocity();

    velocity += dv;

    model.setVelocity(velocity);

    m_acceleratedInLastFrame = true;
}

std::unique_ptr<EntityController> PlayerController::clone() const
{
    return std::make_unique<PlayerController>(*this);
}
std::unique_ptr<EntityController> PlayerController::create(Entity* owner) const
{
    return std::make_unique<PlayerController>(owner, nullptr);
}
