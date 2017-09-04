#include "tiles/models/WeaponTileModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

#include "configuration/ResourceConfigurationReaders.h"
#include "configuration/SlotContentRequirementConfigurationReader.h"
#include "configuration/CommonConfigurationReaders.h"

#include "Player.h"

#include "World.h"

#include "Rng.h"

REGISTER_TILE_MODEL_TYPE(WeaponTileModel)

WeaponTileModel::WeaponTileModel(Tile& owner, CommonData& commonData) :
    TileModel(owner),
    m_commonData(&commonData)
{
}
WeaponTileModel::WeaponTileModel(const WeaponTileModel& other, Tile& owner) :
    TileModel(other, owner),
    m_commonData(other.m_commonData),
    m_attributes(other.m_attributes),
    m_quality(other.m_quality)
{
}
WeaponTileModel::~WeaponTileModel()
{
}

void WeaponTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");

    ConfigurationReaders::read(m_commonData->validSlots, config["validSlots"]);
    ConfigurationReaders::read(m_commonData->correctSlots, config["correctSlots"]);

    m_commonData->attributeRandomizer.loadFromConfiguration(config["attributeRandomizationGuidelines"]);

    m_commonData->maxQuantity = config["maxQuantity"].getDefault<int>(1);
    m_commonData->drag = config["drag"].get<float>();
    m_commonData->maxThrowDistance = config["maxThrowDistance"].getDefault<int>(0);
    m_commonData->canBeStored = config["canBeStored"].getDefault<bool>(false);

    m_commonData->requiresAmmo = config["requiresAmmo"].get<bool>();
    if (m_commonData->requiresAmmo)
    {
        m_commonData->ammoPerAttack = config["ammoPerAttack"].get<int>();
        ConfigurationNode allowedAmmoGroupsConfig = config["allowedAmmoGroups"];
        const int numAllowedAmmoGroups = allowedAmmoGroupsConfig.length();
        for (int i = 1; i <= numAllowedAmmoGroups; ++i)
        {
            m_commonData->allowedAmmoGroups.emplace_back(allowedAmmoGroupsConfig[i].get<std::string>());
        }
    }
    else
    {
        m_commonData->chanceToBreak = config["chanceToBreak"].getDefault<float>(0.0f);
        ConfigurationReaders::read(m_commonData->projectile, config["projectile"]);
    }

    m_commonData->raritySelector.loadFromConfiguration(config["raritySelector"]);
    m_commonData->prefixSelector.loadFromConfiguration(config["prefixSelector"]);

    m_commonData->lightDimming = config["lightDimming"].getDefault<float>(1.0f);
    m_commonData->useTime = config["useTime"].get<float>();
    m_commonData->cooldown = config["cooldown"].get<float>();
}
TileRarity WeaponTileModel::rarity() const
{
    return m_commonData->raritySelector.select(m_quality);
}
float WeaponTileModel::lightDimming() const
{
    return m_commonData->lightDimming;
}
std::string WeaponTileModel::prefix() const
{
    return m_commonData->prefixSelector.select(m_quality);
}

bool WeaponTileModel::equals(const TileModel& other) const
{
    const WeaponTileModel& otherModel = static_cast<const WeaponTileModel&>(other);

    return m_attributes == otherModel.m_attributes;

    return true;
}
bool WeaponTileModel::isMovable() const
{
    return true;
}
bool WeaponTileModel::isThrowableThrough() const
{
    return true;
}
bool WeaponTileModel::allowsTilesAbove() const
{
    return true;
}
int WeaponTileModel::maxThrowDistance() const
{
    return m_commonData->maxThrowDistance;
}
bool WeaponTileModel::canBeStored() const
{
    return true;
}
int WeaponTileModel::maxQuantity() const
{
    return m_commonData->maxQuantity;
}
bool WeaponTileModel::isSlotValid(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
}
bool WeaponTileModel::isSlotCorrect(SlotContentRequirement req) const
{
    return m_commonData->correctSlots.count(req) > 0;
}
const std::string& WeaponTileModel::displayedName() const
{
    return m_commonData->displayedName;
}
const AttributeSet& WeaponTileModel::attributes() const
{
    return m_attributes;
}

float WeaponTileModel::drag() const
{
    return m_commonData->drag;
}

void WeaponTileModel::onTileInstantiated()
{
    auto result = m_commonData->attributeRandomizer.randomize();
    m_attributes = std::move(result.attributes);
    m_quality = result.quality;
}

TileAttackResult WeaponTileModel::attack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity)
{
    TileAttackResult attackResult{ 0, 0, m_commonData->useTime, m_commonData->cooldown };

    if (m_commonData->requiresAmmo)
    {
        TileStack& ammo = player.ammo();
        if (ammo.isEmpty()) return attackResult;
        if (ammo.quantity() < m_commonData->ammoPerAttack) return attackResult;
        if (std::find(m_commonData->allowedAmmoGroups.begin(), m_commonData->allowedAmmoGroups.end(), ammo.tile().model().ammoGroup()) == m_commonData->allowedAmmoGroups.end()) return attackResult;

        ammo.indirectAttack(world, player, hintedPosition);
        attackResult.ammoUsed = m_commonData->ammoPerAttack;
    }
    else
    {
        world.spawnProjectile(m_commonData->projectile.get(), world, player.entity(), hintedPosition);

        if (Rng<std::ranlux48>::instance().doesHappen(m_commonData->chanceToBreak))
        {
            attackResult.weaponUsed = 1;
        }
    }

    return attackResult;
}

std::unique_ptr<TileModel> WeaponTileModel::clone(Tile& owner) const
{
    return std::make_unique<WeaponTileModel>(*this, owner);
}
