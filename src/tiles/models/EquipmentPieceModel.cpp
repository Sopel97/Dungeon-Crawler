#include "tiles/models/EquipmentPieceModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

#include "GameTime.h"

REGISTER_TILE_MODEL_TYPE(EquipmentPieceModel)

EquipmentPieceModel::EquipmentPieceModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData)
{
}
EquipmentPieceModel::EquipmentPieceModel(const EquipmentPieceModel& other, Tile& owner) :
    TileModel(other, owner),
    m_commonData(other.m_commonData),
    m_attributes(other.m_attributes),
    m_quality(other.m_quality)
{
}
EquipmentPieceModel::~EquipmentPieceModel()
{
}

void EquipmentPieceModel::loadFromConfiguration(ConfigurationNode& config)
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

    ConfigurationNode lightConfig = config["light"];
    if (lightConfig.exists())
    {
        m_commonData->light = OscillatingLightSource::fromConfig(lightConfig);
    }
    else
    {
        m_commonData->light = std::nullopt;
    }

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);

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
TileRarity EquipmentPieceModel::rarity() const
{
    return m_commonData->raritySelector.select(m_quality);
}
float EquipmentPieceModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::optional<Light> EquipmentPieceModel::light(const ls::Vec2I& pos) const
{
    if (m_commonData->light.has_value())
    {
        return Light(
            m_commonData->light.value().at(GameTime::instance().now(), static_cast<double>(reinterpret_cast<intptr_t>(this))),
            static_cast<ls::Vec2F>(pos) + ls::Vec2F(0.5f, 0.5f),
            this
        );
    }
    else
    {
        return std::nullopt;
    }
}
std::string EquipmentPieceModel::prefix() const
{
    return m_commonData->prefixSelector.select(m_quality);
}

bool EquipmentPieceModel::equals(const TileModel& other) const
{
    const EquipmentPieceModel& otherModel = static_cast<const EquipmentPieceModel&>(other);

    return m_attributes == otherModel.m_attributes;

    return true;
}
bool EquipmentPieceModel::isMovable() const
{
    return true;
}
bool EquipmentPieceModel::isThrowableThrough() const
{
    return true;
}
bool EquipmentPieceModel::allowsTilesAbove() const
{
    return true;
}
int EquipmentPieceModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool EquipmentPieceModel::canBeStored() const
{
    return true;
}
int EquipmentPieceModel::maxQuantity() const
{
    return 1;
}
bool EquipmentPieceModel::isSlotValid(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
}
bool EquipmentPieceModel::isSlotCorrect(SlotContentRequirement req) const
{
    return m_commonData->correctSlots.count(req) > 0;
}
const std::string& EquipmentPieceModel::displayedName() const
{
    return m_commonData->displayedName;
}
const AttributeSet& EquipmentPieceModel::attributes() const
{
    return m_attributes;
}

float EquipmentPieceModel::drag() const
{
    return m_commonData->drag;
}

void EquipmentPieceModel::onTileInstantiated()
{
    auto result = m_commonData->attributeRandomizer.randomize();
    m_attributes = std::move(result.attributes);
    m_quality = result.quality;
}

std::unique_ptr<TileModel> EquipmentPieceModel::clone(Tile& owner) const
{
    return std::make_unique<EquipmentPieceModel>(*this, owner);
}
