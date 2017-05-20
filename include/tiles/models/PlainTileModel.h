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
    virtual ~PlainTileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool hasCollider() const;
    virtual const ls::Rectangle2F& collider() const;
    virtual bool isMovableFrom() const;
    virtual bool isThrowableThrough() const;
    virtual bool isMovableTo() const;
    virtual bool canBeStored() const;

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
        int maxThrowDistance;
        bool isThrowableThrough;
        bool isMovableTo;
        bool canBeStored;
    };
    CommonData* m_commonData;
};

REGISTER_TILE_MODEL_TYPE(PlainTileModel)

#endif // PLAINTILEMODEL_H
