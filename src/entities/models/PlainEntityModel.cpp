#include "entities/models/PlainEntityModel.h"

#include "OscillatingLightSource.h"

#include "GameTime.h"

using namespace ls;

REGISTER_ENTITY_MODEL_TYPE(PlainEntityModel)

PlainEntityModel::PlainEntityModel(Entity& owner, CommonData& commonData) :
    EntityModel(owner),
    m_commonData(&commonData),
    m_position(0, 0),
    m_velocity(0, 0),
    m_directionOfMove(Direction::South),
    m_distanceTravelled(0.0f),
    m_health(1)
{

}
PlainEntityModel::PlainEntityModel(const PlainEntityModel& other, Entity& owner) :
    EntityModel(other, owner),
    m_commonData(other.m_commonData),
    m_position(other.m_position),
    m_velocity(other.m_velocity),
    m_directionOfMove(other.m_directionOfMove),
    m_distanceTravelled(other.m_distanceTravelled),
    m_health(other.m_health)
{

}
PlainEntityModel::~PlainEntityModel()
{

}

void PlainEntityModel::loadFromConfiguration(ConfigurationNode& config)
{
    m_health = m_commonData->maxHealth = config["maxHealth"].get<int>();
    m_commonData->maxSpeed = config["maxSpeed"].get<float>();
    m_commonData->corpseTile = ResourceManager<TilePrefab>::instance().get(config["corpseTile"].get<std::string>());
    m_commonData->lootRandomizer.loadFromConfiguration(config["lootRandomizationGuidelines"]);
    m_commonData->group = AggroGroupIdHelper::stringToEnum(config["group"].get<std::string>());

    ConfigurationNode attributesConfig = config["attributes"];
    const int numAttributes = attributesConfig.length();
    for (int i = 1; i <= numAttributes; ++i)
    {
        const std::string attributeName = attributesConfig[i][1].get<std::string>();
        const AttributeId attributeId = AttributeIdHelper::stringToEnum(attributeName);
        const int value = attributesConfig[i][2];
        m_commonData->attributes += Attribute{ attributeId, value };
    }
}

EntityCollider PlainEntityModel::collider()
{
    return EntityCollider(*m_owner, ls::Circle2F(m_position, 6.0f / 32.0f));
}

const Vec2F& PlainEntityModel::position() const
{
    return m_position;
}
void PlainEntityModel::setPosition(const ls::Vec2F& newPosition)
{
    m_position = newPosition;
}
const Vec2F& PlainEntityModel::velocity() const
{
    return m_velocity;
}
void PlainEntityModel::setVelocity(const ls::Vec2F& newVelocity)
{
    m_velocity = newVelocity;
}

float PlainEntityModel::distanceTravelled() const
{
    return m_distanceTravelled;
}
int PlainEntityModel::health() const
{
    return m_health;
}
int PlainEntityModel::maxHealth() const
{
    return m_commonData->maxHealth;
}
void PlainEntityModel::setHealth(int newHealth)
{
    m_health = newHealth;
}
TileStack PlainEntityModel::createCorpse() const
{
    TileStack corpse = TileStack(m_commonData->corpseTile->instantiate(), 1);
    m_commonData->lootRandomizer.randomize(*(corpse.tile().model().inventory()));
    return corpse;
}
AggroGroupId PlainEntityModel::group() const
{
    return m_commonData->group;
}
const AttributeArray& PlainEntityModel::attributes() const
{
    return m_commonData->attributes;
}
std::optional<Light> PlainEntityModel::light() const
{
    return OscillatingLightSource(
        Light(m_position, 4.0f, sf::Color::Green, this),
        Light(m_position, 3.5f, sf::Color::Yellow, this),
        1.0
    ).at(GameTime::instance().now(), static_cast<double>(reinterpret_cast<intptr_t>(this))); //TODO: better way to get per entity constant value
}

EntityModel::Direction PlainEntityModel::directionOfMove() const
{
    return m_directionOfMove;
}
void PlainEntityModel::update(World& world, float dt)
{
    //--m_health;

    m_position += m_velocity * dt;
}

void PlainEntityModel::accelerate(const ls::Vec2F& dv)
{
    m_velocity += dv;
}
std::unique_ptr<EntityModel> PlainEntityModel::clone(Entity& owner) const
{
    return std::make_unique<PlainEntityModel>(*this, owner);
}
