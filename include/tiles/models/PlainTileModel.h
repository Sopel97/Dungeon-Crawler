#ifndef PLAINTILEMODEL_H
#define PLAINTILEMODEL_H

#include "TileModel.h"

#include "ResourceLoaders.h"

#include <memory>

class Tile;

class PlainTileModel : public TileModel
{
public:
    PlainTileModel(Tile* owner);
    PlainTileModel(const PlainTileModel& other);
    virtual ~PlainTileModel();

    virtual std::unique_ptr<TileModel> clone() const;
    virtual std::unique_ptr<TileModel> create(Tile* owner) const;
protected:
};

REGISTER_TILE_MODEL_TYPE(PlainTileModel)

#endif // PLAINTILEMODEL_H
