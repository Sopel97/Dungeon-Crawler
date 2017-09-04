#pragma once

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "configuration/Configuration.h"

#include "../LibS/Geometry.h"

#include "ComponentCommonData.h"

#include "AttributeSet.h"
#include "AttributeRandomizer.h"

#include "OscillatingLightSource.h"

#include <memory>

class Tile;
class TileLocation;

class EffectGivingTileModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        std::optional<OscillatingLightSource> light;
        ResourceHandle<TilePrefab> transformsIntoTile;
        ResourceHandle<EffectPrefab> effect;
        std::optional<float> effectDuration;
        float drag;
        int maxThrowDistance;
        bool canBeStored;
        TileRarity rarity;
        float lightDimming;
        int maxQuantity;
        bool isConsumable;
    };

    EffectGivingTileModel(Tile& owner, CommonData& commonData);
    EffectGivingTileModel(const EffectGivingTileModel& other, Tile& owner);
    ~EffectGivingTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool equals(const TileModel& other) const override;

    bool isMovable() const override;
    bool isThrowableThrough() const override;
    bool allowsTilesAbove() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    const std::string& displayedName() const override;
    TileRarity rarity() const override;
    float lightDimming() const override;
    std::optional<Light> light(const ls::Vec2I& pos) const override;

    TileUseResult use(Player& player, const TileLocation& location, int quantity) override;
    TileUseResult use(Player& player, const InventorySlotLocation& location, int quantity) override;

    float drag() const override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

private:
    TileUseResult use(Player& player);
};