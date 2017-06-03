#include "tiles/models/EquipmentPieceModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

EquipmentPieceModel::EquipmentPieceModel() :
    TileModel(),
    m_commonData(nullptr)
{
}
EquipmentPieceModel::EquipmentPieceModel(const EquipmentPieceModel& other) :
    TileModel(other),
    m_commonData(other.m_commonData),
    m_attributes(other.m_attributes)
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
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
}

bool EquipmentPieceModel::hasCollider() const
{
    return false;
}
bool EquipmentPieceModel::isMovableFrom() const
{
    return true;
}
bool EquipmentPieceModel::isThrowableThrough() const
{
    return true;
}
bool EquipmentPieceModel::isMovableTo() const
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
bool EquipmentPieceModel::meetsRequirements(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
}
const std::string& EquipmentPieceModel::displayedName() const
{
    return m_commonData->displayedName;
}
const TileAttributeSet& EquipmentPieceModel::attributes() const
{
    return m_attributes;
}

float EquipmentPieceModel::drag() const
{
    return m_commonData->drag;
}

void EquipmentPieceModel::onTileInstantiated()
{
    m_attributes = m_commonData->attributeRandomizer.randomize();
}

std::unique_ptr<ComponentCommonData> EquipmentPieceModel::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}
void EquipmentPieceModel::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}

std::unique_ptr<TileModel> EquipmentPieceModel::clone() const
{
    return std::make_unique<EquipmentPieceModel>(*this);
}
