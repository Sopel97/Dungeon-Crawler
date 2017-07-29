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

class AmmoTileModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        std::set<SlotContentRequirement> validSlots;
        std::set<SlotContentRequirement> correctSlots;
        AttributeRandomizer attributeRandomizer;
        int maxQuantity;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
        TileAmmoGroupType ammoGroup;
        ResourceHandle<ProjectilePrefab> projectile;
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
    bool meetsRequirements(SlotContentRequirement req) const override;
    const std::string& displayedName() const override;
    const AttributeSet& attributes() const override;

    float drag() const override;

    void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity) override;

    TileAmmoGroupType ammoGroup() const override;

    void onTileInstantiated() override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    AttributeSet m_attributes;
};