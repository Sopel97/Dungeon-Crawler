#include "PlainTileModel.h"

#include "../LibS/make_unique.h"

using namespace Geo;

PlainTileModel::PlainTileModel(Tile* owner) :
    TileModel(owner),
    m_commonData(std::make_shared<CommonData>())
{

}
PlainTileModel::PlainTileModel(const PlainTileModel& other) :
    TileModel(other),
    m_commonData(other.m_commonData)
{

}
PlainTileModel::~PlainTileModel()
{

}

void PlainTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode colliderConfig = config["collider"];
    if(colliderConfig.exists())
    {
        m_commonData->hasCollider = true;
        m_commonData->collider = RectangleF(
                                     Vec2F(colliderConfig[1][1].get<float>(), colliderConfig[1][2].get<float>()),
                                     Vec2F(colliderConfig[2][1].get<float>(), colliderConfig[2][2].get<float>())
                                 );
    }
    else
    {
        m_commonData->hasCollider = false;
    }

    m_commonData->drag = config["drag"].get<float>();
}

bool PlainTileModel::hasCollider() const
{
    return m_commonData->hasCollider;
}
const RectangleF& PlainTileModel::collider() const
{
    return m_commonData->collider;
}

float PlainTileModel::drag() const
{
    return  m_commonData->drag;
}

void PlainTileModel::onTilePlaced(const TileLocation& location)
{

}
void PlainTileModel::onTileRemoved(const TileLocation& location)
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
