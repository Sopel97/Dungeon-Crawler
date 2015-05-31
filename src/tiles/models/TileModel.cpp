#include "TileModel.h"


TileModel::TileModel(Tile* owner) :
    m_owner(owner)
{

}
TileModel::TileModel(const TileModel& other) :
    m_owner(other.m_owner)
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

void TileModel::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}
