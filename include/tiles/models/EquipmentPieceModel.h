#pragma once

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include "AttributeSet.h"
#include "AttributeRandomizer.h"

#include <memory>

class Tile;
class TileLocation;

class EquipmentPieceModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        std::set<SlotContentRequirement> validSlots;
        std::set<SlotContentRequirement> correctSlots;
        AttributeRandomizer attributeRandomizer;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
    };

    EquipmentPieceModel(Tile& owner, CommonData& commonData);
    EquipmentPieceModel(const EquipmentPieceModel& other, Tile& owner);
    ~EquipmentPieceModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool hasCollider() const override;
    bool isMovableFrom() const override;
    bool isThrowableThrough() const override;
    bool isMovableTo() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    bool meetsRequirements(SlotContentRequirement req) const override;
    const std::string& displayedName() const override;
    const AttributeSet& attributes() const override;

    float drag() const override;

    void onTileInstantiated() override;

    static std::unique_ptr<ComponentCommonData> createCommonDataStorage();

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* m_commonData;

    AttributeSet m_attributes;
};