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
    PlainTileModel();
    PlainTileModel(const PlainTileModel& other);
    ~PlainTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool hasCollider() const override;
    const ls::Rectangle2F& collider() const override;
    bool isMovableFrom() const override;
    bool isThrowableThrough() const override;
    bool isMovableTo() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    const std::string& displayedName() const override;

    float drag() const;

    std::unique_ptr<ComponentCommonData> createCommonDataStorage() const override;
    void setCommonDataStorage(ComponentCommonData& commonData) override;

    std::unique_ptr<TileModel> clone() const override;
protected:
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
    CommonData* m_commonData;
};

REGISTER_TILE_MODEL_TYPE(PlainTileModel)

#endif // PLAINTILEMODEL_H
