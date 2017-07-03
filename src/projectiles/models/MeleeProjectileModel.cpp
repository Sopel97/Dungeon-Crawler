#include "projectiles/models/MeleeProjectileModel.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "ResourceLoaders.h"

#include "DamageCalculator.h"

REGISTER_PROJECTILE_MODEL_TYPE(MeleeProjectileModel);

MeleeProjectileModel::MeleeProjectileModel(Projectile& owner, ComponentCommonData* commonData) :
    ProjectileModel(owner),
    m_position(0, 0),
    m_radius(15.0f),
    m_offset(15.0f),
    m_health(3),
    m_group(AggroGroupId::Neutral),
    m_attributes()
{

}
MeleeProjectileModel::MeleeProjectileModel(const MeleeProjectileModel& other, Projectile& owner) :
    ProjectileModel(other, owner),
    m_position(other.m_position),
    m_radius(other.m_radius),
    m_offset(other.m_offset),
    m_health(other.m_health),
    m_group(other.m_group),
    m_attributes(other.m_attributes)
{

}
MeleeProjectileModel::~MeleeProjectileModel()
{

}

void MeleeProjectileModel::loadFromConfiguration(ConfigurationNode& config)
{

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
    return nullptr;
}

void MeleeProjectileModel::onProjectileInstantiated(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    m_group = parentEntity.model().group();
    
    ls::Vec2F displacement = hintedPosition - parentEntity.model().position();
    if (displacement.magnitudeSquared() < 0.001f) displacement.x = 1.0f;
    displacement.normalize();
    const ls::Vec2F offset = displacement * m_offset;

    m_position = parentEntity.model().position() + offset;

    m_attributes = TileAttributeArray(parentEntity.model().attributes(), { TileAttributeId::Attack });
}
void MeleeProjectileModel::onCollidedWithEntity(EntityCollider& entityCollider)
{
    Entity& entity = entityCollider.entity();
    if (!EntityGroupRelations::canDamage(m_group, entity.model().group())) return;
    
    --m_health;
    const int damage = DamageCalculator::calculateDamage(m_attributes, entity.model().attributes());
    entity.controller().dealDamage(damage);
}

std::unique_ptr<ProjectileModel> MeleeProjectileModel::clone(Projectile& owner) const
{
    return std::make_unique<MeleeProjectileModel>(*this, owner);
}