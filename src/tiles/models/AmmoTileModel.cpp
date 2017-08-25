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
    m_attributes(other.m_attributes),
    m_quality(other.m_quality)
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
    m_commonData->correctSlots.insert(SlotContentRequirement::None);
    if (correctSlotsConfiguration.exists())
    {
        const int numEntries = correctSlotsConfiguration.length();
        for (int i = 1; i <= numEntries; ++i)
        {
            m_commonData->correctSlots.insert(SlotContentRequirementHelper::stringToEnum(correctSlotsConfiguration[i].get<std::string>()));
        }
    }

    ConfigurationNode attributeParams = config["attributeRandomizationGuidelines"];
    m_commonData->attributeRandomizer.loadFromConfiguration(attributeParams);

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
    m_commonData->maxQuantity = config["maxQuantity"].getDefault<int>(1);
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);

    m_commonData->ammoGroup = config["ammoGroup"].get<std::string>();
    m_commonData->projectile = ResourceManager<ProjectilePrefab>::instance().get(config["projectile"].get<std::string>());

    ConfigurationNode raritySelectorConfig = config["raritySelector"];
    if (raritySelectorConfig.exists())
    {
        m_commonData->raritySelector.loadFromConfiguration(raritySelectorConfig);
    }
    else
    {
        m_commonData->raritySelector = TileRaritySelector::createDefault();
    }

    ConfigurationNode prefixSelectorConfig = config["prefixSelector"];
    if (prefixSelectorConfig.exists())
    {
        m_commonData->prefixSelector.loadFromConfiguration(prefixSelectorConfig);
    }
    else
    {
        m_commonData->prefixSelector = TilePrefixSelector::createDefault();
    }
}
TileRarity AmmoTileModel::rarity() const
{
    return m_commonData->raritySelector.select(m_quality);
}
float AmmoTileModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::string AmmoTileModel::prefix() const
{
    return m_commonData->prefixSelector.select(m_quality);
}

bool AmmoTileModel::equals(const TileModel& other) const
{
    const AmmoTileModel& otherModel = static_cast<const AmmoTileModel&>(other);

    return m_attributes == otherModel.m_attributes;

    return true;
}
bool AmmoTileModel::isMovable() const
{
    return true;
}
bool AmmoTileModel::isThrowableThrough() const
{
    return true;
}
bool AmmoTileModel::allowsTilesAbove() const
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
bool AmmoTileModel::isSlotValid(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
}
bool AmmoTileModel::isSlotCorrect(SlotContentRequirement req) const
{
    return m_commonData->correctSlots.count(req) > 0;
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
void AmmoTileModel::indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity)
{
    world.spawnProjectile(m_commonData->projectile.get(), world, player.entity(), hintedPosition);
}
TileAmmoGroupType AmmoTileModel::ammoGroup() const
{
    return m_commonData->ammoGroup;
}

void AmmoTileModel::onTileInstantiated()
{
    auto result = m_commonData->attributeRandomizer.randomize();
    m_attributes = std::move(result.attributes);
    m_quality = result.quality;
}

std::unique_ptr<TileModel> AmmoTileModel::clone(Tile& owner) const
{
    return std::make_unique<AmmoTileModel>(*this, owner);
}
