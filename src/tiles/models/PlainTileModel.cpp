#include "tiles/models/PlainTileModel.h"

using namespace ls;

PlainTileModel::PlainTileModel() :
    TileModel(),
    m_commonData(nullptr)
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
        m_commonData->collider = Rectangle2F(
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
const Rectangle2F& PlainTileModel::collider() const
{
    return m_commonData->collider;
}

float PlainTileModel::drag() const
{
    return  m_commonData->drag;
}
std::unique_ptr<ComponentCommonData> PlainTileModel::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}

void PlainTileModel::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}


std::unique_ptr<TileModel> PlainTileModel::clone() const
{
    return std::make_unique<PlainTileModel>(*this);
}

