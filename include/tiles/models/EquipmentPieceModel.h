#pragma once

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include "tiles/TileRaritySelector.h"
#include "tiles/TilePrefixSelector.h"

#include "AttributeSet.h"
#include "AttributeRandomizer.h"

#include "OscillatingLightSource.h"

#include <memory>

class Tile;
class TileLocation;

class EquipmentPieceModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        SlotContentRequirementSet validSlots;
        SlotContentRequirementSet correctSlots;
        AttributeRandomizer attributeRandomizer;
        std::optional<OscillatingLightSource> light;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
        TileRaritySelector raritySelector;
        TilePrefixSelector prefixSelector;
        float lightDimming;
    };

    EquipmentPieceModel(Tile& owner, CommonData& commonData);
    EquipmentPieceModel(const EquipmentPieceModel& other, Tile& owner);
    ~EquipmentPieceModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool equals(const TileModel& other) const override;

    bool isMovable() const override;
    bool isThrowableThrough() const override;
    bool allowsTilesAbove() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    bool isSlotValid(SlotContentRequirement req) const override;
    bool isSlotCorrect(SlotContentRequirement req) const override;
    const std::string& displayedName() const override;
    const AttributeSet& attributes() const override;
    TileRarity rarity() const override;
    std::string prefix() const override;
    float lightDimming() const override;
    std::optional<Light> light(const ls::Vec2I& pos) const override;

    float drag() const override;

    void onTileInstantiated() override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    AttributeSet m_attributes;
    float m_quality;
};