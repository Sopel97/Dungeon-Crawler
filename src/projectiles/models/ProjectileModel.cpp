#include "projectiles/models/ProjectileModel.h"

using namespace ls;

ProjectileModel::ProjectileModel(Projectile& owner) :
    ProjectileComponent(owner)
{

}
ProjectileModel::ProjectileModel(const ProjectileModel& other, Projectile& owner) :
    ProjectileComponent(other, owner)
{

}
ProjectileModel::~ProjectileModel()
{

}

void ProjectileModel::loadFromConfiguration(ConfigurationNode& config)
{

}

bool ProjectileModel::canCollideWithEntities() const
{
    return false;
}
bool ProjectileModel::canCollideWithTiles() const
{
    return false;
}
ProjectileCollider ProjectileModel::collider()
{
    return ProjectileCollider(*m_owner, ls::Circle2F());
}
bool ProjectileModel::isDead() const
{
    return true;
}
AggroGroupId ProjectileModel::group() const
{
    return AggroGroupId::Neutral;
}
int ProjectileModel::health() const
{
    return 0;
}
void ProjectileModel::setHealth(int newHealth)
{

}
const ls::Vec2F& ProjectileModel::position() const
{
    static const ls::Vec2F somePosition;
    return somePosition;
}
void ProjectileModel::setPosition(const ls::Vec2F& newPosition)
{
}
const Entity* ProjectileModel::parentEntity() const
{
    return nullptr;
}

void ProjectileModel::update(World& world, float dt)
{

}

std::unique_ptr<ProjectileModel> ProjectileModel::clone(Projectile& owner) const
{
    return std::make_unique<ProjectileModel>(*this, owner);
}
