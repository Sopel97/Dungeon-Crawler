#include "tiles/models/AmmoTileModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

#include "World.h"

#include "Player.h"

REGISTER_TILE_MODEL_TYPE(AmmoTileModel)

AmmoTileModel::AmmoTileModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData)
{
}
AmmoTileModel::AmmoTileModel(const AmmoTileModel& other, Tile& owner) :
    TileModel(other, owner),
    m_commonData(other.m_commonData),
    m_attributes(other.m_attributes)
{
}
AmmoTileModel::~AmmoTileModel()
{
}

void AmmoTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode validSlotsConfiguration = config["validSlots"];
    m_commonData->validSlots.insert(SlotContentRequirement::None);
    if (validSlotsConfiguration.exists())
    {
        const int numEntries = validSlotsConfiguration.length();
        for (int i = 1; i <= numEntries; ++i)
        {
            m_commonData->validSlots.insert(SlotContentRequirementHelper::stringToEnum(validSlotsConfiguration[i].get<std::string>()));
        }
    }

    ConfigurationNode correctSlotsConfiguration = config["correctSlots"];
    m_commonData->validSlots.insert(SlotContentRequirement::None);
    if (correctSlotsConfiguration.exists())
    {
        const int numEntries = correctSlotsConfiguration.length();
        for (int i = 1; i <= numEntries; ++i)
        {
            m_commonData->validSlots.insert(SlotContentRequirementHelper::stringToEnum(correctSlotsConfiguration[i].get<std::string>()));
        }
    }

    ConfigurationNode attributeParams = config["attributeRandomizationGuidelines"];
    m_commonData->attributeRandomizer.loadFromConfiguration(attributeParams);

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
    m_commonData->maxQuantity = config["maxQuantity"].getDefault<int>(1);
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);

    m_commonData->ammoGroup = config["ammoGroup"].get<std::string>();
    m_commonData->projectile = ResourceManager::instance().get<ProjectilePrefab>(config["projectile"].get<std::string>());
}

bool AmmoTileModel::equals(const TileModel& other) const
{
    // TODO: compare better

    return true;
}
bool AmmoTileModel::isMovableFrom() const
{
    return true;
}
bool AmmoTileModel::isThrowableThrough() const
{
    return true;
}
bool AmmoTileModel::isMovableTo() const
{
    return true;
}
int AmmoTileModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool AmmoTileModel::canBeStored() const
{
    return true;
}
int AmmoTileModel::maxQuantity() const
{
    return m_commonData->maxQuantity;
}
bool AmmoTileModel::meetsRequirements(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
}
const std::string& AmmoTileModel::displayedName() const
{
    return m_commonData->displayedName;
}
const AttributeSet& AmmoTileModel::attributes() const
{
    return m_attributes;
}

float AmmoTileModel::drag() const
{
    return m_commonData->drag;
}
void AmmoTileModel::indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{
    world.spawnProjectile(m_commonData->projectile.get(), world, player.entity(), hintedPosition);
}
TileAmmoGroupType AmmoTileModel::ammoGroup() const
{
    return m_commonData->ammoGroup;
}

void AmmoTileModel::onTileInstantiated()
{
    m_attributes = m_commonData->attributeRandomizer.randomize();
}

std::unique_ptr<TileModel> AmmoTileModel::clone(Tile& owner) const
{
    return std::make_unique<AmmoTileModel>(*this, owner);
}