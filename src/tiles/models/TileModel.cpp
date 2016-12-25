#include "tiles/models/TileModel.h"

using namespace ls;

const Rectangle2F TileModel::m_emptyCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));

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
bool TileModel::equals(const TileModel& other) const
{
    return true;
}

void TileModel::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}

bool TileModel::hasCollider() const
{
    return false;
}
const Rectangle2F& TileModel::collider() const
{
    return m_emptyCollider;
}

Inventory* TileModel::inventory()
{
    return nullptr;
}

float TileModel::drag() const
{
    return 1.0f;
}

void TileModel::onTilePlaced(const TileLocation& location)
{

}
void TileModel::onTileRemoved(const TileLocation& location)
{

}
void TileModel::onTileQuantityChanged(int oldQuantity, int newQuantity)
{

}

std::unique_ptr<TileModel> TileModel::clone() const
{
    return std::make_unique<TileModel>(*this);
}

std::unique_ptr<TileModel> TileModel::create(Tile* owner) const
{
    return std::make_unique<TileModel>(owner);
}
