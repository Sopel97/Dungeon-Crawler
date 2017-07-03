#include "projectiles/models/MeleeProjectileModel.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "ResourceLoaders.h"

REGISTER_PROJECTILE_MODEL_TYPE(MeleeProjectileModel);

MeleeProjectileModel::MeleeProjectileModel(Projectile& owner, ComponentCommonData* commonData) :
    ProjectileModel(owner),
    m_position(0, 0),
    m_radius(20.0f),
    m_offset(10.0f),
    m_health(3),
    m_group(AggroGroupId::Neutral)
{

}
MeleeProjectileModel::MeleeProjectileModel(const MeleeProjectileModel& other, Projectile& owner) :
    ProjectileModel(other, owner),
    m_position(other.m_position),
    m_radius(other.m_radius),
    m_offset(other.m_offset),
    m_health(other.m_health),
    m_group(other.m_group)
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
}
void MeleeProjectileModel::onCollidedWithEntity(EntityCollider& entityCollider)
{
    --m_health;
    std::cout << "asdadsads";
}

std::unique_ptr<ProjectileModel> MeleeProjectileModel::clone(Projectile& owner) const
{
    return std::make_unique<MeleeProjectileModel>(*this, owner);
}