#include "entities/controllers/PlayerController.h"

#include "World.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "../LibS/Geometry.h"

#include <cmath>

using namespace ls;

REGISTER_ENTITY_CONTROLLER_TYPE(PlayerController)

PlayerController::PlayerController(Entity& owner) :
    EntityController(owner),
    m_playerOwner(),
    m_acceleratedHorizontallyInLastFrame(false),
    m_acceleratedVerticallyInLastFrame(false)
{

}

PlayerController::PlayerController(Player& player, Entity& owner) :
    EntityController(owner),
    m_playerOwner(&player),
    m_acceleratedHorizontallyInLastFrame(false),
    m_acceleratedVerticallyInLastFrame(false)
{

}
PlayerController::PlayerController(const PlayerController& other, Entity& owner) :
    EntityController(other, owner),
    m_playerOwner(other.m_playerOwner),
    m_acceleratedHorizontallyInLastFrame(other.m_acceleratedHorizontallyInLastFrame),
    m_acceleratedVerticallyInLastFrame(other.m_acceleratedVerticallyInLastFrame)
{

}
PlayerController::~PlayerController()
{

}

void PlayerController::loadFromConfiguration(ConfigurationNode& config)
{

}

void PlayerController::update(World& world, float dt)
{
    auto& model = m_owner->model();
    Vec2F velocity = model.velocity();
    const Vec2F& position = model.position();
    float maxSpeed = model.maxSpeed();

    float speed = velocity.magnitude();
    if(speed > maxSpeed)
    {
        velocity *= maxSpeed / speed;
        speed = maxSpeed;
    }

    float deceleration = 200.0f;
    if(!m_acceleratedHorizontallyInLastFrame && !m_acceleratedVerticallyInLastFrame)
        deceleration /= 1.41f;
    float d = deceleration * dt * world.drag(position);
    if(!m_acceleratedHorizontallyInLastFrame)
    {
        float avx = std::abs(velocity.x);
        if(d > avx)
        {
            velocity.x = 0.0f;
        }
        else
        {
            velocity.x *= (avx - d) / avx;
        }
    }
    if(!m_acceleratedVerticallyInLastFrame)
    {
        float avy = std::abs(velocity.y);
        if(d > avy)
        {
            velocity.y = 0.0f;
        }
        else
        {
            velocity.y *= (avy - d) / avy;
        }
    }

    model.setVelocity(velocity);

    m_acceleratedHorizontallyInLastFrame = false;
    m_acceleratedVerticallyInLastFrame = false;

    move(dt);
}

void PlayerController::move(float dt)
{
    auto& model = m_owner->model();

    Vec2F position = model.position();
    float distanceTravelled = model.distanceTravelled();

    Vec2F displacement = model.velocity() * dt;

    position += displacement;
    distanceTravelled += displacement.magnitude();

    model.setPosition(position);
    model.setDistanceTravelled(distanceTravelled);

    //set direction facing based on displacement if it there. If not just on velocity
    if(displacement.magnitude() > 0.1f)
    {
        if(displacement.x < -0.01f && std::abs(displacement.x) > std::abs(displacement.y)) model.setDirectionOfMovement(EntityModel::Direction::West);
        if(displacement.x > 0.01f && std::abs(displacement.x) > std::abs(displacement.y))  model.setDirectionOfMovement(EntityModel::Direction::East);
        if(displacement.y < -0.01f && std::abs(displacement.y) > std::abs(displacement.x)) model.setDirectionOfMovement(EntityModel::Direction::North);
        if(displacement.y > 0.01f && std::abs(displacement.y) > std::abs(displacement.x))  model.setDirectionOfMovement(EntityModel::Direction::South);
    }
    else
    {
        const Vec2F& velocity = model.velocity();
        if(velocity.magnitude() > 0.1f)
        {
            if(velocity.x < -0.01f && std::abs(velocity.x) > std::abs(velocity.y)) model.setDirectionOfMovement(EntityModel::Direction::West);
            if(velocity.x > 0.01f && std::abs(velocity.x) > std::abs(velocity.y))  model.setDirectionOfMovement(EntityModel::Direction::East);
            if(velocity.y < -0.01f && std::abs(velocity.y) > std::abs(velocity.x)) model.setDirectionOfMovement(EntityModel::Direction::North);
            if(velocity.y > 0.01f && std::abs(velocity.y) > std::abs(velocity.x))  model.setDirectionOfMovement(EntityModel::Direction::South);
        }
    }
}
void PlayerController::accelerate(const ls::Vec2F& dv)
{
    auto& model = m_owner->model();
    Vec2F velocity = model.velocity();

    velocity += dv;

    model.setVelocity(velocity);

    if(std::abs(dv.x) > 0.0f)
    {
        m_acceleratedHorizontallyInLastFrame = true;
    }
    if(std::abs(dv.y) > 0.0f)
    {
        m_acceleratedVerticallyInLastFrame = true;
    }
}

std::unique_ptr<EntityController> PlayerController::clone(Entity& owner) const
{
    return std::make_unique<PlayerController>(*this, owner);
}
