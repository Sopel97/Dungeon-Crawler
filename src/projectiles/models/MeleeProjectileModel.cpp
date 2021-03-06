#include "projectiles/models/MeleeProjectileModel.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "ResourceLoaders.h"

#include "DamageCalculator.h"

REGISTER_PROJECTILE_MODEL_TYPE(MeleeProjectileModel);

MeleeProjectileModel::MeleeProjectileModel(Projectile& owner, CommonData& commonData) :
    ProjectileModel(owner),
    m_commonData(&commonData),
    m_parentEntity(nullptr),
    m_position(0, 0),
    m_offset(0.0f, 0.0f),
    m_radius(0.5f),
    m_health(0),
    m_group(AggroGroupId::Neutral),
    m_attributes(),
    m_timeLeft(0.0f)
{

}
MeleeProjectileModel::MeleeProjectileModel(const MeleeProjectileModel& other, Projectile& owner) :
    ProjectileModel(other, owner),
    m_commonData(other.m_commonData),
    m_parentEntity(other.m_parentEntity),
    m_position(other.m_position),
    m_offset(other.m_offset),
    m_radius(other.m_radius),
    m_health(other.m_health),
    m_group(other.m_group),
    m_attributes(other.m_attributes),
    m_timeLeft(other.m_timeLeft)
{

}
MeleeProjectileModel::~MeleeProjectileModel()
{

}

void MeleeProjectileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode inheritedAttributesConfig = config["inheritedAttributes"];
    const int numEntries = inheritedAttributesConfig.length();
    for (int i = 1; i <= numEntries; ++i)
    {
        const std::string attributeName = inheritedAttributesConfig[i].get<std::string>();
        m_commonData->inheritedAttributes.emplace_back(AttributeIdHelper::stringToEnum(attributeName));
    }

    m_timeLeft = config["lifetime"].get<float>();
    m_commonData->penetration = config["penetration"].getDefault<int>(0);
}

bool MeleeProjectileModel::canCollideWithTiles() const
{
    return false;
}
bool MeleeProjectileModel::canCollideWithEntities() const
{
    return true;
}
ProjectileCollider MeleeProjectileModel::collider()
{
    return ProjectileCollider(*m_owner, ls::Circle2F(m_position, m_radius));
}
bool MeleeProjectileModel::isDead() const
{
    return m_health <= 0;
}
AggroGroupId MeleeProjectileModel::group() const
{
    return m_group;
}

int MeleeProjectileModel::health() const
{
    return m_health;
}
void MeleeProjectileModel::setHealth(int newHealth)
{
    m_health = newHealth;
}
const ls::Vec2F& MeleeProjectileModel::position() const
{
    return m_position;
}
void MeleeProjectileModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}

const Entity* MeleeProjectileModel::parentEntity() const
{
    return m_parentEntity;
}

void MeleeProjectileModel::onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    m_parentEntity = &parentEntity;
    m_health = m_commonData->penetration + 1;

    m_group = parentEntity.model().group();
    
    ls::Vec2F displacement = hintedPosition - parentEntity.model().position();
    if (displacement.magnitudeSquared() < 0.001f) displacement.x = 1.0f;
    displacement.normalize();
    const float offsetLength = m_radius;
    const ls::Vec2F offset = displacement * offsetLength;
    m_offset = offset;

    m_position = parentEntity.model().position() + offset;

    m_attributes = AttributeArray(parentEntity.model().attributes(), m_commonData->inheritedAttributes);
}
void MeleeProjectileModel::onCollidedWithEntity(EntityCollider& entityCollider)
{
    Entity& entity = entityCollider.entity();
    if (std::find(m_hitEntities.begin(), m_hitEntities.end(), &entity) != m_hitEntities.end()) return;

    m_hitEntities.emplace_back(&entity);
    if (!EntityGroupRelations::canDamage(m_group, entity.model().group())) return;
    
    --m_health;
    const int damage = DamageCalculator::calculateDamage(m_attributes, entity.model().attributes());
    entity.model().dealDamage(damage);
}
void MeleeProjectileModel::update(World& world, float dt)
{
    m_timeLeft -= dt;
    if (m_timeLeft <= 0.0f) m_health = 0;

    if (m_parentEntity)
    {
        m_position = m_parentEntity->model().position() + m_offset;
    }
}

void MeleeProjectileModel::onParentEntityDeleted()
{
    m_parentEntity = nullptr;
    m_health = 0;
}
std::unique_ptr<ProjectileModel> MeleeProjectileModel::clone(Projectile& owner) const
{
    return std::make_unique<MeleeProjectileModel>(*this, owner);
}