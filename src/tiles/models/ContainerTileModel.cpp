#include "tiles/models/ContainerTileModel.h"

using namespace ls;

ContainerTileModel::ContainerTileModel(Tile* owner) :
    TileModel(owner),
    m_commonData(std::make_shared<CommonData>()),
    m_inventory(0)
{

}
ContainerTileModel::ContainerTileModel(const ContainerTileModel& other) :
    TileModel(other),
    m_commonData(other.m_commonData),
    m_inventory(other.m_inventory)
{

}
ContainerTileModel::~ContainerTileModel()
{

}

void ContainerTileModel::loadFromConfiguration(ConfigurationNode& config)
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

    m_inventory.setSize(config["inventorySize"].get<int>());
}

bool ContainerTileModel::hasCollider() const
{
    return m_commonData->hasCollider;
}
const RectangleF& ContainerTileModel::collider() const
{
    return m_commonData->collider;
}

Inventory* ContainerTileModel::inventory()
{
    return &m_inventory;
}

float ContainerTileModel::drag() const
{
    return  m_commonData->drag;
}


std::unique_ptr<TileModel> ContainerTileModel::clone() const
{
    return std::make_unique<ContainerTileModel>(*this);
}

std::unique_ptr<TileModel> ContainerTileModel::create(Tile* owner) const
{
    return std::make_unique<ContainerTileModel>(owner);
}
