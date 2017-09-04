#include "tiles/models/EffectGivingTileModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

#include "Player.h"

#include "GameTime.h"

REGISTER_TILE_MODEL_TYPE(EffectGivingTileModel)

EffectGivingTileModel::EffectGivingTileModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData)
{
}
EffectGivingTileModel::EffectGivingTileModel(const EffectGivingTileModel& other, Tile& owner) :
    TileModel(other, owner),
    m_commonData(other.m_commonData)
{
}
EffectGivingTileModel::~EffectGivingTileModel()
{
}

void EffectGivingTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode lightConfig = config["light"];
    if (lightConfig.exists())
    {
        m_commonData->light = OscillatingLightSource::fromConfig(lightConfig);
    }
    else
    {
        m_commonData->light = std::nullopt;
    }

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);
    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);
    m_commonData->rarity = TileRarity(config["rarity"].getDefault<int>(1));
    m_commonData->maxQuantity = config["maxQuantity"].getDefault<int>(1);

    ConfigurationNode transformsIntoConfig = config["transformsInto"];
    if (transformsIntoConfig.exists())
    {
        m_commonData->transformsIntoTile = ResourceManager<TilePrefab>::instance().get(transformsIntoConfig.get<std::string>());
    }
    else
    {
        m_commonData->transformsIntoTile = nullptr;
    }

    m_commonData->effect = ResourceManager<EffectPrefab>::instance().get(config["effect"]);
    
    ConfigurationNode effectDurationConfig = config["effectDuration"];
    if (effectDurationConfig.exists())
    {
        m_commonData->effectDuration = effectDurationConfig.get<float>();
    }
    else
    {
        m_commonData->effectDuration = std::nullopt;
    }

    m_commonData->isConsumable = config["isConsumable"].getDefault<bool>(true);
}
TileRarity EffectGivingTileModel::rarity() const
{
    return m_commonData->rarity;
}
float EffectGivingTileModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::optional<Light> EffectGivingTileModel::light(const ls::Vec2I& pos) const
{
    if (m_commonData->light.has_value())
    {
        return Light(
            m_commonData->light.value().at(GameTime::instance().now(), static_cast<double>(reinterpret_cast<intptr_t>(this))),
            static_cast<ls::Vec2F>(pos) + ls::Vec2F(0.5f, 0.5f),
            this
        );
    }
    else
    {
        return std::nullopt;
    }
}

bool EffectGivingTileModel::equals(const TileModel& other) const
{
    return true;
}
bool EffectGivingTileModel::isMovable() const
{
    return true;
}
bool EffectGivingTileModel::isThrowableThrough() const
{
    return true;
}
bool EffectGivingTileModel::allowsTilesAbove() const
{
    return true;
}
int EffectGivingTileModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool EffectGivingTileModel::canBeStored() const
{
    return true;
}
int EffectGivingTileModel::maxQuantity() const
{
    return m_commonData->maxQuantity;
}
const std::string& EffectGivingTileModel::displayedName() const
{
    return m_commonData->displayedName;
}

float EffectGivingTileModel::drag() const
{
    return m_commonData->drag;
}

TileUseResult EffectGivingTileModel::use(Player& player, const TileLocation& location, int quantity)
{
    return use(player);
}
TileUseResult EffectGivingTileModel::use(Player& player, const InventorySlotLocation& location, int quantity)
{
    return use(player);
}
TileUseResult EffectGivingTileModel::use(Player& player)
{
    player.addEffect(m_commonData->effect.get().instantiate(m_commonData->effectDuration));

    TileUseResult result = TileUseResult::noAction();
    
    if (m_commonData->isConsumable)
    {
        result.numUsed = 1;
    }

    if (m_commonData->transformsIntoTile)
    {
        result.tileCreationRequests.push_back(TileUseResult::TileCreateRequest{m_commonData->transformsIntoTile, 1});
    }
    
    return result;
}

std::unique_ptr<TileModel> EffectGivingTileModel::clone(Tile& owner) const
{
    return std::make_unique<EffectGivingTileModel>(*this, owner);
}
