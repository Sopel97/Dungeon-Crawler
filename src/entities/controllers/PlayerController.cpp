#include "PlayerController.h"

#include "World.h"

#include "Entity.h"
#include "EntityModel.h"

#include "../LibS/make_unique.h"

#include "../LibS/GeometryLight.h"

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
    if(!m_acceleratedInLastFrame)
    {
        velocity *= 0.8;
        if(velocity.magnitude() < 1.0f) velocity = Vec2F(0.0f, 0.0f);
    }

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
    constexpr float maxSpeed = 64.0f;
    auto& model = m_owner->model();
    Vec2F velocity = model.velocity();

    if(velocity.x < -0.01f && std::abs(velocity.x) > std::abs(velocity.y)) model.setDirectionOfMove(EntityModel::Direction::West);
    if(velocity.x > 0.01f && std::abs(velocity.x) > std::abs(velocity.y))  model.setDirectionOfMove(EntityModel::Direction::East);
    if(velocity.y < -0.01f && std::abs(velocity.y) > std::abs(velocity.x)) model.setDirectionOfMove(EntityModel::Direction::North);
    if(velocity.y > 0.01f && std::abs(velocity.y) > std::abs(velocity.x))  model.setDirectionOfMove(EntityModel::Direction::South);

    velocity += dv;
    if(velocity.magnitude() > maxSpeed)
    {
        velocity.normalize();
        velocity *= maxSpeed;
    }

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
