#include "projectiles/models/MovingProjectileModel.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "ResourceLoaders.h"

#include "OscillatingLightSource.h"
#include "GameTime.h"

#include "DamageCalculator.h"

REGISTER_PROJECTILE_MODEL_TYPE(MovingProjectileModel);

MovingProjectileModel::MovingProjectileModel(Projectile& owner, CommonData& commonData) :
    ProjectileModel(owner),
    m_commonData(&commonData),
    m_position(0, 0),
    m_velocity(0, 0),
    m_group(AggroGroupId::Neutral),
    m_health(0),
    m_distanceTravelled(0.0f),
    m_timeTravelled(0.0f),
    m_attributes()
{

}
MovingProjectileModel::MovingProjectileModel(const MovingProjectileModel& other, Projectile& owner) :
    ProjectileModel(other, owner),
    m_commonData(other.m_commonData),
    m_position(other.m_position),
    m_velocity(other.m_velocity),
    m_group(other.m_group),
    m_health(other.m_health),
    m_distanceTravelled(other.m_distanceTravelled),
    m_timeTravelled(other.m_timeTravelled),
    m_attributes(other.m_attributes)
{

}
MovingProjectileModel::~MovingProjectileModel()
{

}

void MovingProjectileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode inheritedAttributesConfig = config["inheritedAttributes"];
    const int numEntries = inheritedAttributesConfig.length();
    for (int i = 1; i <= numEntries; ++i)
    {
        const std::string attributeName = inheritedAttributesConfig[i].get<std::string>();
        m_commonData->inheritedAttributes.emplace_back(AttributeIdHelper::stringToEnum(attributeName));
    }

    ConfigurationNode lightConfig = config["light"];
    if (lightConfig.exists())
    {
        ConfigurationNode light1Config = lightConfig["light1"];
        const float rad1 = light1Config["radius"].get<float>();
        const int r1 = light1Config["color"][1].get<int>();
        const int g1 = light1Config["color"][2].get<int>();
        const int b1 = light1Config["color"][3].get<int>();

        ConfigurationNode light2Config = lightConfig["light2"];
        const float rad2 = light2Config["radius"].get<float>();
        const int r2 = light2Config["color"][1].get<int>();
        const int g2 = light2Config["color"][2].get<int>();
        const int b2 = light2Config["color"][3].get<int>();

        const float freq = lightConfig["freq"].get<float>();

        m_commonData->light = CommonData::OscillatingLight{
            CommonData::Light{ rad1, sf::Color(r1, g1, b1) },
            CommonData::Light{ rad2, sf::Color(r2, g2, b2) },
            freq
        };
    }
    else
    {
        m_commonData->light = std::nullopt;
    }

    m_commonData->initialSpeed = config["initialSpeed"].get<float>();
    m_commonData->radius = config["radius"].get<float>();
    m_commonData->acceleration = config["acceleration"].get<float>();
    m_commonData->maxTravelDistance = config["maxTravelDistance"].get<float>();
    m_commonData->maxTravelTime = config["maxTravelTime"].get<float>();
    m_commonData->penetration = config["penetration"].getDefault<int>(0);
}

bool MovingProjectileModel::canCollideWithTiles() const
{
    return true;
}
bool MovingProjectileModel::canCollideWithEntities() const
{
    return true;
}
ProjectileCollider MovingProjectileModel::collider()
{
    return ProjectileCollider(*m_owner, ls::Circle2F(m_position, m_commonData->radius));
}
bool MovingProjectileModel::isDead() const
{
    return m_health <= 0;
}
AggroGroupId MovingProjectileModel::group() const
{
    return m_group;
}

int MovingProjectileModel::health() const
{
    return m_health;
}
void MovingProjectileModel::setHealth(int newHealth)
{
    m_health = newHealth;
}
std::optional<Light> MovingProjectileModel::light() const
{
    if (m_commonData->light.has_value())
    {
        const auto& light1 = m_commonData->light.value().light1;
        const auto& light2 = m_commonData->light.value().light2;
        const float freq = m_commonData->light.value().freq;

        return OscillatingLightSource(
            Light(m_position, light1.radius, light1.color, this),
            Light(m_position, light2.radius, light2.color, this),
            freq
        ).at(GameTime::instance().now(), static_cast<double>(reinterpret_cast<intptr_t>(this))); //TODO: better way to get per entity constant value;
    }
    else
    {
        return std::nullopt;
    }
}
const ls::Vec2F& MovingProjectileModel::position() const
{
    return m_position;
}
ls::Vec2F MovingProjectileModel::velocity() const
{
    return m_velocity;
}
void MovingProjectileModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}

const Entity* MovingProjectileModel::parentEntity() const
{
    return nullptr;
}

void MovingProjectileModel::onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    m_group = parentEntity.model().group();
    m_health = m_commonData->penetration + 1;

    ls::Vec2F displacement = hintedPosition - parentEntity.model().position();
    if (displacement.magnitudeSquared() < 0.001f) displacement.x = 1.0f;
    displacement.normalize();
    const float offsetLength = m_commonData->radius;
    const ls::Vec2F offset = displacement * offsetLength;

    m_position = parentEntity.model().position() + offset;
    m_velocity = displacement * m_commonData->initialSpeed + parentEntity.model().velocity();

    m_attributes = AttributeArray(parentEntity.model().attributes(), m_commonData->inheritedAttributes);
}
void MovingProjectileModel::onCollidedWithEntity(EntityCollider& entityCollider)
{
    Entity& entity = entityCollider.entity();
    if (std::find(m_hitEntities.begin(), m_hitEntities.end(), &entity) != m_hitEntities.end()) return;

    m_hitEntities.emplace_back(&entity);
    if (!EntityGroupRelations::canDamage(m_group, entity.model().group())) return;

    --m_health;
    const int damage = DamageCalculator::calculateDamage(m_attributes, entity.model().attributes());
    entity.model().dealDamage(damage);
}
void MovingProjectileModel::onCollidedWithTile(TileCollider& tileCollider)
{
    m_health = 0;
}
void MovingProjectileModel::update(World& world, float dt)
{
    constexpr float minSpeed = 0.1f;

    const float speed = m_velocity.magnitude();
    const float newSpeed = std::max(0.0f, speed - m_commonData->acceleration * dt);

    if (newSpeed > minSpeed)
    {
        m_velocity *= newSpeed / speed;
        m_position += m_velocity * dt;

        m_timeTravelled += dt;
        m_distanceTravelled += newSpeed * dt;
        if (m_timeTravelled > m_commonData->maxTravelTime || m_distanceTravelled > m_commonData->maxTravelDistance)
        {
            m_health = 0;
        }
    }
    else
    {
        m_health = 0;
    }
}

std::unique_ptr<ProjectileModel> MovingProjectileModel::clone(Projectile& owner) const
{
    return std::make_unique<MovingProjectileModel>(*this, owner);
}