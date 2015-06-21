#include "PlainTileModel.h"

#include "../LibS/make_unique.h"

PlainTileModel::PlainTileModel(Tile* owner) :
    TileModel(owner)
{

}
PlainTileModel::PlainTileModel(const PlainTileModel& other) :
    TileModel(other)
{

}
PlainTileModel::~PlainTileModel()
{

}

std::unique_ptr<TileModel> PlainTileModel::clone() const
{
    return std::make_unique<PlainTileModel>(*this);
}

std::unique_ptr<TileModel> PlainTileModel::create(Tile* owner) const
{
    return std::make_unique<PlainTileModel>(owner);
}
