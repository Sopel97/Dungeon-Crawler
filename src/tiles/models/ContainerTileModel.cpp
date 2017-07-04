#include "tiles/models/ContainerTileModel.h"

#include "GameConstants.h"

using namespace ls;

REGISTER_TILE_MODEL_TYPE(ContainerTileModel)

ContainerTileModel::ContainerTileModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData),
    m_inventory(1)
{

}
ContainerTileModel::ContainerTileModel(const ContainerTileModel& other, Tile& owner) :
    TileModel(other, owner),
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
        m_commonData->collider = Rectangle2F(
                                     Vec2F(colliderConfig[1][1].get<float>(), colliderConfig[1][2].get<float>()),
                                     Vec2F(colliderConfig[2][1].get<float>(), colliderConfig[2][2].get<float>())
                                 );
    }
    else
    {
        m_commonData->hasCollider = false;
    }

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->isThrowableThrough = config["isThrowableThrough"].getDefault<bool>(false);
    m_commonData->isMovableTo = config["isMovableTo"].getDefault<bool>(false);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);

    m_inventory.setSize(config["inventorySize"].get<int>());
}

bool ContainerTileModel::hasCollider() const
{
    return m_commonData->hasCollider;
}
TileCollider ContainerTileModel::collider(const ls::Vec2I& pos)
{
    const ls::Rectangle2F aabb = m_commonData->collider.translated(static_cast<ls::Vec2F>(pos) * GameConstants::tileSize);
    return TileCollider(*m_owner, aabb);
}
bool ContainerTileModel::isMovableFrom() const
{
    return m_commonData->maxThrowDistance > 0;
}
bool ContainerTileModel::isThrowableThrough() const
{
    return m_commonData->isThrowableThrough;
}
bool ContainerTileModel::isMovableTo() const
{
    return m_commonData->isMovableTo;
}
int ContainerTileModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool ContainerTileModel::canBeStored() const
{
    return m_commonData->canBeStored;
}
int ContainerTileModel::maxQuantity() const
{
    return 1;
}
bool ContainerTileModel::meetsRequirements(SlotContentRequirement req) const
{
    return req == SlotContentRequirement::None
        || req == SlotContentRequirement::LeftHand
        || req == SlotContentRequirement::RightHand
        || req == SlotContentRequirement::Container;
}
const std::string& ContainerTileModel::displayedName() const
{
    return m_commonData->displayedName;
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
std::unique_ptr<ComponentCommonData> ContainerTileModel::createCommonDataStorage()
{
    return std::make_unique<CommonData>();
}

std::unique_ptr<TileModel> ContainerTileModel::clone(Tile& owner) const
{
    return std::make_unique<ContainerTileModel>(*this, owner);
}
