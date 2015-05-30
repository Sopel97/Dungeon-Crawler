#ifndef PLAINTILEMODEL_H
#define PLAINTILEMODEL_H

#include "TileModel.h"

class Tile;

class PlainTileModel : public TileModel
{
public:
    PlainTileModel(Tile* owner);
    virtual ~PlainTileModel();
protected:
};

#endif // PLAINTILEMODEL_H
