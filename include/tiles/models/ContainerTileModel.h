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
    virtual ~ContainerTileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool hasCollider() const;
    virtual const ls::Rectangle2F& collider() const;
    virtual bool isMovableFrom() const;
    virtual bool isThrowableThrough() const;
    virtual bool isMovableTo() const;

    virtual Inventory* inventory();
    virtual const Inventory* inventory() const;

    virtual float drag() const;

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileModel> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        bool hasCollider;
        ls::Rectangle2F collider;
        float drag;
        bool isMovableFrom;
        bool isThrowableThrough;
        bool isMovableTo;
    };
    CommonData* m_commonData;

    ContainerInventory m_inventory;
};

REGISTER_TILE_MODEL_TYPE(ContainerTileModel)

#endif // CONTAINERTILEMODEL_H
