#ifndef PLAINTILEMODEL_H
#define PLAINTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <memory>

class Tile;

class PlainTileModel : public TileModel
{
public:
    struct CommonData : public ComponentCommonData
    {
        std::string displayedName;
        bool hasCollider;
        ls::Rectangle2F collider;
        std::optional<ls::Rectangle2F> lightOccluder;
        float drag;
        int maxThrowDistance;
        bool isThrowableThrough;
        bool allowsTilesAbove;
        bool canBeStored;
        int maxQuantity;
        TileRarity rarity;
        float lightDimming;
    };

    PlainTileModel(Tile& owner, CommonData& commonData);
    PlainTileModel(const PlainTileModel& other, Tile& owner);
    ~PlainTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    std::optional<TileCollider> collider(const ls::Vec2I& pos) override;
    std::optional<ls::Rectangle2F> lightOccluder(const ls::Vec2I& pos) const override;
    bool isMovable() const override;
    bool isThrowableThrough() const override;
    bool allowsTilesAbove() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    const std::string& displayedName() const override;
    TileRarity rarity() const override;
    float lightDimming() const override;

    float drag() const;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;
};

#endif // PLAINTILEMODEL_H
