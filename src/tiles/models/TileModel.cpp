#include "TileModel.h"

#include "../LibS/make_unique.h"

using namespace Geo;

const RectangleF TileModel::m_emptyCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));

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

bool TileModel::hasCollider() const
{
    return false;
}
const RectangleF& TileModel::collider() const
{
    return m_emptyCollider;
}

std::unique_ptr<TileModel> TileModel::clone() const
{
    return std::make_unique<TileModel>(*this);
}

std::unique_ptr<TileModel> TileModel::create(Tile* owner) const
{
    return std::make_unique<TileModel>(owner);
}
