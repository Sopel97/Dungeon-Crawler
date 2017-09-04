#include "tiles/models/PlainTileModel.h"

#include "ConfigurationShapesLoaders.h"
#include "ConfigurationOptionalLoader.h"

#include "World.h"

#include "GameTime.h"

using namespace ls;

REGISTER_TILE_MODEL_TYPE(PlainTileModel)

PlainTileModel::PlainTileModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData)
{

}
PlainTileModel::PlainTileModel(const PlainTileModel& other, Tile& owner) :
    TileModel(other, owner),
    m_commonData(other.m_commonData)
{

}
PlainTileModel::~PlainTileModel()
{

}

void PlainTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");

    ConfigurationLoaders::load(m_commonData->collider, config["collider"]);
    ConfigurationLoaders::load(m_commonData->lightOccluder, config["lightOccluder"]);
    ConfigurationLoaders::load(m_commonData->light, config["light"]);

    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->isThrowableThrough = config["isThrowableThrough"].getDefault<bool>(false);
    m_commonData->allowsTilesAbove = config["allowsTilesAbove"].getDefault<bool>(false);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
    m_commonData->maxQuantity = config["maxQuantity"].getDefault<int>(1);
    m_commonData->rarity = TileRarity(config["rarity"].getDefault<int>(1));
    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);
}

std::optional<TileCollider> PlainTileModel::collider(const ls::Vec2I& pos)
{
    if (!m_commonData->collider.has_value()) return std::nullopt;

    const ls::Rectangle2F aabb = m_commonData->collider.value().translated(static_cast<ls::Vec2F>(pos));
    return TileCollider(*m_owner, aabb);
}
std::optional<ls::Rectangle2F> PlainTileModel::lightOccluder(const ls::Vec2I& pos) const
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
TileRarity PlainTileModel::rarity() const
{
    return m_commonData->rarity;
}
float PlainTileModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::optional<Light> PlainTileModel::light(const ls::Vec2I& pos) const
{
    if (m_commonData->light.has_value())
    {
        return Light(
            m_commonData->light.value().now(OscillatingLightSource::seedFromPointer(this)),
            static_cast<ls::Vec2F>(pos) + ls::Vec2F(0.5f, 0.5f),
            this
        );
    }
    else
    {
        return std::nullopt;
    }
}
bool PlainTileModel::isMovable() const
{
    return m_commonData->maxThrowDistance > 0;
}
bool PlainTileModel::isThrowableThrough() const
{
    return m_commonData->isThrowableThrough;
}
bool PlainTileModel::allowsTilesAbove() const
{
    return m_commonData->allowsTilesAbove;
}
int PlainTileModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool PlainTileModel::canBeStored() const
{
    return m_commonData->canBeStored;
}
int PlainTileModel::maxQuantity() const
{
    return m_commonData->maxQuantity;
}
const std::string& PlainTileModel::displayedName() const
{
    return m_commonData->displayedName;
}

float PlainTileModel::drag() const
{
    return  m_commonData->drag;
}

std::unique_ptr<TileModel> PlainTileModel::clone(Tile& owner) const
{
    return std::make_unique<PlainTileModel>(*this, owner);
}

