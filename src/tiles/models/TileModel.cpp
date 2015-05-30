#include "TileModel.h"


TileModel::TileModel(Tile* owner) :
    m_owner(owner)
{

}
TileModel::~TileModel()
{

}

void TileModel::loadFromConfiguration(ConfigurationNode& config)
{

}

const Tile* TileModel::owner() const
{
    return m_owner;
}
