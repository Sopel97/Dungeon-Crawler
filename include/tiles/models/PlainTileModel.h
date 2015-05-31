#ifndef PLAINTILEMODEL_H
#define PLAINTILEMODEL_H

#include "TileModel.h"

#include <memory>

class Tile;

class PlainTileModel : public TileModel
{
public:
    PlainTileModel(Tile* owner);
    PlainTileModel(const PlainTileModel& other);
    virtual ~PlainTileModel();

    virtual std::unique_ptr<TileModel> clone() const;
protected:
};

#endif // PLAINTILEMODEL_H
