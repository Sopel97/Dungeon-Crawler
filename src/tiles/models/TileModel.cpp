#include "tiles/models/TileModel.h"

using namespace ls;

const Rectangle2F TileModel::m_emptyCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));
TileModel::TileModel() :
    TileComponent()
{

}
TileModel::TileModel(const TileModel& other) :
    TileComponent(other)
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
bool TileModel::isThrowableThrough() const
{
    return false;
}
bool TileModel::isMovableTo() const
{
    return false;
}
int TileModel::maxThrowDistance() const
{
    return 0;
}
bool TileModel::canBeStored() const
{
    return false;
}
int TileModel::maxQuantity() const
{
    return 1;
}
bool TileModel::meetsRequirements(SlotContentRequirement req) const
{
    return req == SlotContentRequirement::None
        || req == SlotContentRequirement::LeftHand
        || req == SlotContentRequirement::RightHand;
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

std::unique_ptr<TileModel> TileModel::clone() const
{
    return std::make_unique<TileModel>(*this);
}
