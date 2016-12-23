#ifndef CONTAINERTILEMODEL_H
#define CONTAINERTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "ContainerInventory.h"

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include <memory>

class Tile;
class TileLocation;

class ContainerTileModel : public TileModel
{
public:
    ContainerTileModel(Tile* owner);
    ContainerTileModel(const ContainerTileModel& other);
    virtual ~ContainerTileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool hasCollider() const;
    virtual const ls::Rectangle2F& collider() const;

    virtual Inventory* inventory();

    virtual float drag() const;

    virtual std::unique_ptr<TileModel> clone() const;
    virtual std::unique_ptr<TileModel> create(Tile* owner) const;
protected:
    struct CommonData
    {
        bool hasCollider;
        ls::Rectangle2F collider;
        float drag;
    };
    std::shared_ptr<CommonData> m_commonData;

    ContainerInventory m_inventory;
};

REGISTER_TILE_MODEL_TYPE(ContainerTileModel)

#endif // CONTAINERTILEMODEL_H
