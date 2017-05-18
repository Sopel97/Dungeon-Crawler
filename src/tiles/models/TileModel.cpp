#include "tiles/models/TileModel.h"

using namespace ls;

const Rectangle2F TileModel::m_emptyCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));
TileModel::TileModel() :
    Component()
{

}
TileModel::TileModel(const TileModel& other) :
    Component(other)
{

}
TileModel::~TileModel()
{

}

bool TileModel::equals(const TileModel& other) const
{
    return true;
}

bool TileModel::hasCollider() const
{
    return false;
}
const Rectangle2F& TileModel::collider() const
{
    return m_emptyCollider;
}
bool TileModel::isMovableFrom() const
{
    return false;
}
bool TileModel::isMovableTo() const
{
    return false;
}

Inventory* TileModel::inventory()
{
    return nullptr;
}
const Inventory* TileModel::inventory() const
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
void TileModel::onTileCloned()
{

}

std::unique_ptr<TileModel> TileModel::clone() const
{
    return std::make_unique<TileModel>(*this);
}
