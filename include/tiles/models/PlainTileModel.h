#ifndef PLAINTILEMODEL_H
#define PLAINTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include "../LibS/GeometryLight.h"

#include <memory>

class Tile;

class PlainTileModel : public TileModel
{
public:
    PlainTileModel(Tile* owner);
    PlainTileModel(const PlainTileModel& other);
    virtual ~PlainTileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool hasCollider() const;
    virtual const Geo::RectangleF& collider() const;

    virtual std::unique_ptr<TileModel> clone() const;
    virtual std::unique_ptr<TileModel> create(Tile* owner) const;
protected:
    struct CommonData
    {
        bool hasCollider;
        Geo::RectangleF collider;
    };
    std::shared_ptr<CommonData> m_commonData;
};

REGISTER_TILE_MODEL_TYPE(PlainTileModel)

#endif // PLAINTILEMODEL_H
