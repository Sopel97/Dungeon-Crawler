#pragma once

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "configuration/Configuration.h"

#include "../LibS/Geometry.h"

#include "ComponentCommonData.h"

#include "tiles/TileRaritySelector.h"
#include "tiles/TilePrefixSelector.h"

#include "AttributeSet.h"
#include "AttributeRandomizer.h"

#include <memory>

class Tile;
class TileLocation;

class AmmoTileModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        SlotContentRequirementSet validSlots;
        SlotContentRequirementSet correctSlots;
        AttributeRandomizer attributeRandomizer;
        int maxQuantity;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
        TileAmmoGroupType ammoGroup;
        ResourceHandle<ProjectilePrefab> projectile;
        TileRaritySelector raritySelector;
        TilePrefixSelector prefixSelector;
        float lightDimming;
    };

    AmmoTileModel(Tile& owner, CommonData& commonData);
    AmmoTileModel(const AmmoTileModel& other, Tile& owner);
    ~AmmoTileModel() override;

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

    float drag() const override;

    void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity) override;

    TileAmmoGroupType ammoGroup() const override;

    void onTileInstantiated() override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    AttributeSet m_attributes;
    float m_quality;
};