#include "tiles/models/ContainerTileModel.h"

using namespace ls;

ContainerTileModel::ContainerTileModel() :
    TileModel(),
    m_commonData(nullptr),
    m_inventory(1)
{

}
ContainerTileModel::ContainerTileModel(const ContainerTileModel& other) :
    TileModel(other),
    m_commonData(other.m_commonData),
    m_inventory(other.m_inventory.size())
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
    m_commonData->isMovableFrom = config["isMovableFrom"].getDefault<bool>(false);
    m_commonData->isThrowableThrough = config["isThrowableThrough"].getDefault<bool>(false);
    m_commonData->isMovableTo = config["isMovableTo"].getDefault<bool>(false);

    m_inventory.setSize(config["inventorySize"].get<int>());
}

bool ContainerTileModel::hasCollider() const
{
    return m_commonData->hasCollider;
}
const Rectangle2F& ContainerTileModel::collider() const
{
    return m_commonData->collider;
}
bool ContainerTileModel::isMovableFrom() const
{
    return m_commonData->isMovableFrom;
}
bool ContainerTileModel::isThrowableThrough() const
{
    return m_commonData->isThrowableThrough;
}
bool ContainerTileModel::isMovableTo() const
{
    return m_commonData->isMovableTo;
}

Inventory* ContainerTileModel::inventory()
{
    return &m_inventory;
}
const Inventory* ContainerTileModel::inventory() const
{
    return &m_inventory;
}


float ContainerTileModel::drag() const
{
    return  m_commonData->drag;
}
std::unique_ptr<ComponentCommonData> ContainerTileModel::createCommonDataStorage() const
{
    return std::make_unique<CommonData>();
}

void ContainerTileModel::setCommonDataStorage(ComponentCommonData& commonData)
{
    m_commonData = static_cast<CommonData*>(&commonData);
}


std::unique_ptr<TileModel> ContainerTileModel::clone() const
{
    return std::make_unique<ContainerTileModel>(*this);
}
