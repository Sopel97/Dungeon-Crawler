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

class WeaponTileModel : public TileModel
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
        bool requiresAmmo;
        int ammoPerAttack;
        float chanceToBreak;
        std::vector<TileAmmoGroupType> allowedAmmoGroups;
        ResourceHandle<ProjectilePrefab> projectile;
    };

    WeaponTileModel(Tile& owner, CommonData& commonData);
    WeaponTileModel(const WeaponTileModel& other, Tile& owner);
    ~WeaponTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool equals(const TileModel& other) const override;

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

    AttackResult attack(World& world, Player& player, const ls::Vec2F& hintedPosition) override;

    void onTileInstantiated() override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    AttributeSet m_attributes;
};