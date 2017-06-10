#ifndef CONTAINERTILEMODEL_H
#define CONTAINERTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "ContainerInventory.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <memory>

class Tile;
class TileLocation;

class ContainerTileModel : public TileModel
{
public:
    ContainerTileModel();
    ContainerTileModel(const ContainerTileModel& other);
    ~ContainerTileModel() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    bool hasCollider() const override;
    const ls::Rectangle2F& collider() const override;
    bool isMovableFrom() const override;
    bool isThrowableThrough() const override;
    bool isMovableTo() const override;
    int maxThrowDistance() const override;
    bool canBeStored() const override;
    int maxQuantity() const override;
    bool meetsRequirements(SlotContentRequirement req) const override;
    const std::string& displayedName() const override;

    Inventory* inventory() override;
    const Inventory* inventory() const override;

    float drag() const override;

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
    };
    CommonData* m_commonData;

    ContainerInventory m_inventory;
};

REGISTER_TILE_MODEL_TYPE(ContainerTileModel)

#endif // CONTAINERTILEMODEL_H
