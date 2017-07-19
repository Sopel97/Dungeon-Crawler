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
        float drag;
        int maxThrowDistance;
        bool isThrowableThrough;
        bool isMovableTo;
        bool canBeStored;
        int maxQuantity;
    };

    PlainTileModel(Tile& owner, CommonData& commonData);
    PlainTileModel(const PlainTileModel& other, Tile& owner);
    ~PlainTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    std::optional<TileCollider> collider(const ls::Vec2I& pos) override;
    bool isMovableFrom() const override;
    bool isThrowableThrough() const override;
    bool isMovableTo() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    const std::string& displayedName() const override;

    float drag() const;

    std::unique_ptr<TileModel> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;
};

#endif // PLAINTILEMODEL_H
