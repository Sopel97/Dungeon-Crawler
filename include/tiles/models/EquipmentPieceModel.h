#pragma once

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include "tiles/TileAttributeSet.h"
#include "tiles/TileAttributeRandomizer.h"

#include <memory>

class Tile;
class TileLocation;

class EquipmentPieceModel : public TileModel
{
public:
    EquipmentPieceModel();
    EquipmentPieceModel(const EquipmentPieceModel& other);
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

    float drag() const override;

    void onTileInstantiated() override;

    std::unique_ptr<ComponentCommonData> createCommonDataStorage() const override;
    void setCommonDataStorage(ComponentCommonData& commonData) override;

    std::unique_ptr<TileModel> clone() const override;
protected:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        std::set<SlotContentRequirement> validSlots;
        TileAttributeRandomizer attributeRandomizer;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
    };
    CommonData* m_commonData;

    TileAttributeSet m_attributes;
};

REGISTER_TILE_MODEL_TYPE(EquipmentPieceModel)