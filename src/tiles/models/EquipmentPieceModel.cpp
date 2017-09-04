#include "tiles/models/EquipmentPieceModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

#include "configuration/SlotContentRequirementConfigurationReader.h"
#include "configuration/OscillatingLightSourceConfigurationReader.h"
#include "configuration/CommonConfigurationReaders.h"

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
    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");

    ConfigurationReaders::read(m_commonData->validSlots, config["validSlots"]);
    ConfigurationReaders::read(m_commonData->correctSlots, config["correctSlots"]);

    m_commonData->attributeRandomizer.loadFromConfiguration(config["attributeRandomizationGuidelines"]);

    ConfigurationReaders::read(m_commonData->light, config["light"]);

    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);

    m_commonData->raritySelector.loadFromConfiguration(config["raritySelector"]);
    m_commonData->prefixSelector.loadFromConfiguration(config["prefixSelector"]);

    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);
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
            m_commonData->light.value().now(OscillatingLightSource::seedFromPointer(this)),
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
