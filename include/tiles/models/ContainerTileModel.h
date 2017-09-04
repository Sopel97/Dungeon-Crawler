#ifndef CONTAINERTILEMODEL_H
#define CONTAINERTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "ContainerInventory.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <memory>
#include <optional>

class Tile;
class TileLocation;
class TileInformation;

class ContainerTileModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        std::optional<ls::Rectangle2F> collider;
        std::optional<ls::Rectangle2F> lightOccluder;
        float drag;
        int maxThrowDistance;
        bool isThrowableThrough;
        bool allowsTilesAbove;
        bool canBeStored;
        TileRarity rarity;
        float lightDimming;
    };

    ContainerTileModel(Tile& owner, CommonData& commonData);
    ContainerTileModel(const ContainerTileModel& other, Tile& owner);
    ~ContainerTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    std::optional<TileCollider> collider(const ls::Vec2I& pos) override;
    std::optional<ls::Rectangle2F> lightOccluder(const ls::Vec2I& pos) const override;
    bool isMovable() const override;
    bool isThrowableThrough() const override;
    bool allowsTilesAbove() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    bool isSlotValid(SlotContentRequirement req) const override;
    const std::string& displayedName() const override;
    TileInformation additionalInformation() const override;
    TileRarity rarity() const override;
    std::optional<Light> light(const ls::Vec2I& pos) const override;
    float lightDimming() const override;

    Inventory* inventory() override;
    const Inventory* inventory() const override;

    float drag() const override;

    TileUseResult use(Player& player, const TileLocation& location, int quantity) override;
    TileUseResult use(Player& player, const InventorySlotLocation& slot, int quantity) override;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    ContainerInventory m_inventory;
};

#endif // CONTAINERTILEMODEL_H
