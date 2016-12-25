#include "tiles/models/TileModel.h"

using namespace ls;

const Rectangle2F TileModel::m_emptyCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));
TileModel::TileModel() :
    m_owner(nullptr)
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
void TileModel::onTileInstantiated()
{

}

std::unique_ptr<ComponentCommonData> TileModel::createCommonDataStorage() const
{
    return nullptr;
}
void TileModel::setCommonDataStorage(ComponentCommonData& commonData)
{

}

std::unique_ptr<TileModel> TileModel::clone() const
{
    return std::make_unique<TileModel>(*this);
}
