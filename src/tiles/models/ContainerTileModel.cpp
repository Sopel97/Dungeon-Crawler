#include "tiles/models/ContainerTileModel.h"

#include "tiles/TileInformation.h"

#include "Player.h"

#include "configuration/ShapeConfigurationReaders.h"
#include "configuration/CommonConfigurationReaders.h"

#include "World.h"

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
    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");

    ConfigurationReaders::read(m_commonData->collider, config["collider"]);
    ConfigurationReaders::read(m_commonData->lightOccluder, config["lightOccluder"]);

    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->isThrowableThrough = config["isThrowableThrough"].getDefault<bool>(false);
    m_commonData->allowsTilesAbove = config["allowsTilesAbove"].getDefault<bool>(false);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
    m_commonData->rarity = TileRarity(config["rarity"].getDefault<int>(1));
    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);

    m_inventory.setSize(config["inventorySize"].get<int>());
}

std::optional<TileCollider> ContainerTileModel::collider(const ls::Vec2I& pos)
{
    if (!m_commonData->collider.has_value()) return std::nullopt;

    const ls::Rectangle2F aabb = m_commonData->collider.value().translated(static_cast<ls::Vec2F>(pos));
    return TileCollider(*m_owner, aabb);
}
std::optional<ls::Rectangle2F> ContainerTileModel::lightOccluder(const ls::Vec2I& pos) const
{
    if (m_commonData->lightOccluder.has_value())
    {
        return m_commonData->lightOccluder.value().translated(static_cast<ls::Vec2F>(pos));
    }
    else
    {
        return std::nullopt;
    }
}
TileRarity ContainerTileModel::rarity() const
{
    return m_commonData->rarity;
}
float ContainerTileModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::optional<Light> ContainerTileModel::light(const ls::Vec2I& pos) const
{
    // TEST
    // return Light(static_cast<Vec2F>(pos) + ls::Vec2F(0.5f, 0.5f), 2.0f, sf::Color::White, this);

    return std::nullopt;
}
bool ContainerTileModel::isMovable() const
{
    return m_commonData->maxThrowDistance > 0;
}
bool ContainerTileModel::isThrowableThrough() const
{
    return m_commonData->isThrowableThrough;
}
bool ContainerTileModel::allowsTilesAbove() const
{
    return m_commonData->allowsTilesAbove;
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
bool ContainerTileModel::isSlotValid(SlotContentRequirement req) const
{
    return req == SlotContentRequirement::None
        || req == SlotContentRequirement::Container;
}
const std::string& ContainerTileModel::displayedName() const
{
    return m_commonData->displayedName;
}
TileInformation ContainerTileModel::additionalInformation() const
{
    return TileInformation({ { std::string("Capacity: ") + std::to_string(m_inventory.size()), sf::Color::Green } });
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

TileUseResult ContainerTileModel::use(Player& player, const TileLocation& location, int quantity)
{
    player.tryInteractWithExternalInventory(owner(), *(m_owner->model().inventory()), location);

    return TileUseResult::noAction();
}

TileUseResult ContainerTileModel::use(Player& player, const InventorySlotLocation& slot, int quantity)
{
    player.tryInteractWithInternalInventory(owner(), *(m_owner->model().inventory()), slot);

    return TileUseResult::noAction();
}

std::unique_ptr<TileModel> ContainerTileModel::clone(Tile& owner) const
{
    return std::make_unique<ContainerTileModel>(*this, owner);
}
