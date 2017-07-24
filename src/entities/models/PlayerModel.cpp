#include "entities/models/PlayerModel.h"

#include "Player.h"

#include "AttributeArray.h"
#include "AttributeSet.h"

#include "GameTime.h"

#include "World.h"

#include "OscillatingLightSource.h"

using namespace ls;

REGISTER_ENTITY_MODEL_TYPE(PlayerModel)

PlayerModel::PlayerModel(Entity& owner) :
    EntityModel(owner),
    m_playerOwner(),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f)
{

}
PlayerModel::PlayerModel(Player& player, Entity& owner) :
    EntityModel(owner),
    m_playerOwner(&player),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f)
{

}
PlayerModel::PlayerModel(const PlayerModel& other, Entity& owner) :
    EntityModel(other, owner),
    m_playerOwner(other.m_playerOwner),
    m_directionOfMove(other.m_directionOfMove),
    m_distanceTravelled(other.m_distanceTravelled)
{

}
PlayerModel::~PlayerModel()
{

}

void PlayerModel::loadFromConfiguration(ConfigurationNode& config)
{

}

EntityCollider PlayerModel::collider()
{
    return EntityCollider(*m_owner, ls::Circle2F(m_position, 6.0f));
}

const Vec2F& PlayerModel::position() const
{
    return m_position;
}
void PlayerModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlayerModel::velocity() const
{
    return m_velocity;
}
void PlayerModel::setVelocity(const ls::Vec2F& newVelocity)
{
    m_velocity = newVelocity;
}

float PlayerModel::distanceTravelled() const
{
    return m_distanceTravelled;
}
void PlayerModel::setDistanceTravelled(float newDistanceTravelled)
{
    m_distanceTravelled = newDistanceTravelled;
}
AggroGroupId PlayerModel::group() const
{
    return AggroGroupId::Friendly;
}
TileStack& PlayerModel::ammo()
{
    return m_playerOwner->ammo();
}
const AttributeArray& PlayerModel::attributes() const
{
    // TODO: this should be a member and updated when needed
    static AttributeArray attributes;

    attributes.clear();
    for (const auto& tileStack : m_playerOwner->equipmentInventory().contents())
    {
        if (tileStack.isEmpty()) continue;

        const Tile& tile = tileStack.tile();

        const AttributeSet& tileAttributes = tile.model().attributes();
        for (const auto& attribute : tileAttributes)
        {
            attributes += attribute;
        }
    }

    return attributes;
}
std::optional<Light> PlayerModel::light() const
{
    return OscillatingLightSource(
        Light(m_position, 128.0f, sf::Color::Red),
        Light(m_position, 100.0f, sf::Color::Blue),
        0.5
    ).at(GameTime::instance().now());
}

float PlayerModel::maxSpeed() const
{
    return 64.0f;
}

EntityModel::Direction PlayerModel::directionOfMove() const
{
    return m_directionOfMove;
}
void PlayerModel::setDirectionOfMovement(EntityModel::Direction newDirection)
{
    m_directionOfMove = newDirection;
}
void PlayerModel::update(World& world, float dt)
{
    float speed = m_velocity.magnitude();
    if (speed > maxSpeed())
    {
        m_velocity *= maxSpeed() / speed;
        speed = maxSpeed();
    }

    float deceleration = 200.0f;
    if (!m_acceleratedHorizontallyInLastFrame && !m_acceleratedVerticallyInLastFrame)
        deceleration /= 1.41f;
    float d = deceleration * dt * world.drag(m_position);
    if (!m_acceleratedHorizontallyInLastFrame)
    {
        float avx = std::abs(m_velocity.x);
        if (d > avx)
        {
            m_velocity.x = 0.0f;
        }
        else
        {
            m_velocity.x *= (avx - d) / avx;
        }
    }
    if (!m_acceleratedVerticallyInLastFrame)
    {
        float avy = std::abs(m_velocity.y);
        if (d > avy)
        {
            m_velocity.y = 0.0f;
        }
        else
        {
            m_velocity.y *= (avy - d) / avy;
        }
    }

    m_acceleratedHorizontallyInLastFrame = false;
    m_acceleratedVerticallyInLastFrame = false;

    move(dt);
}

void PlayerModel::move(float dt)
{
    Vec2F displacement = m_velocity * dt;

    m_position += displacement;
    m_distanceTravelled += displacement.magnitude();

    //set direction facing based on displacement if it there. If not just on velocity
    if (displacement.magnitude() > 0.1f)
    {
        if (displacement.x < -0.01f && std::abs(displacement.x) > std::abs(displacement.y)) m_directionOfMove = EntityModel::Direction::West;
        if (displacement.x > 0.01f && std::abs(displacement.x) > std::abs(displacement.y))  m_directionOfMove = EntityModel::Direction::East;
        if (displacement.y < -0.01f && std::abs(displacement.y) > std::abs(displacement.x)) m_directionOfMove = EntityModel::Direction::North;
        if (displacement.y > 0.01f && std::abs(displacement.y) > std::abs(displacement.x))  m_directionOfMove = EntityModel::Direction::South;
    }
    else
    {
        if (m_velocity.magnitude() > 0.1f)
        {
            if (m_velocity.x < -0.01f && std::abs(m_velocity.x) > std::abs(m_velocity.y)) m_directionOfMove = EntityModel::Direction::West;
            if (m_velocity.x > 0.01f && std::abs(m_velocity.x) > std::abs(m_velocity.y))  m_directionOfMove = EntityModel::Direction::East;
            if (m_velocity.y < -0.01f && std::abs(m_velocity.y) > std::abs(m_velocity.x)) m_directionOfMove = EntityModel::Direction::North;
            if (m_velocity.y > 0.01f && std::abs(m_velocity.y) > std::abs(m_velocity.x))  m_directionOfMove = EntityModel::Direction::South;
        }
    }
}
void PlayerModel::accelerate(const ls::Vec2F& dv)
{
    m_velocity += dv;

    if (std::abs(dv.x) > 0.0f)
    {
        m_acceleratedHorizontallyInLastFrame = true;
    }
    if (std::abs(dv.y) > 0.0f)
    {
        m_acceleratedVerticallyInLastFrame = true;
    }
}
std::unique_ptr<EntityModel> PlayerModel::clone(Entity& owner) const
{
    return std::make_unique<PlayerModel>(*this, owner);
}
