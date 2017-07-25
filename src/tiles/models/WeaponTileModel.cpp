#include "tiles/models/WeaponTileModel.h"

#include <algorithm>

#include "SlotContentRequirement.h"

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
    m_attributes(other.m_attributes)
{
}
WeaponTileModel::~WeaponTileModel()
{
}

void WeaponTileModel::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode validSlotsConfiguration = config["validSlots"];
    m_commonData->validSlots.insert(SlotContentRequirement::None);
    if (validSlotsConfiguration.exists())
    {
        const int numEntries = validSlotsConfiguration.length();
        for (int i = 1; i <= numEntries; ++i)
        {
            m_commonData->validSlots.insert(SlotContentRequirementHelper::stringToEnum(validSlotsConfiguration[i].get<std::string>()));
        }
    }

    ConfigurationNode correctSlotsConfiguration = config["correctSlots"];
    m_commonData->validSlots.insert(SlotContentRequirement::None);
    if (correctSlotsConfiguration.exists())
    {
        const int numEntries = correctSlotsConfiguration.length();
        for (int i = 1; i <= numEntries; ++i)
        {
            m_commonData->validSlots.insert(SlotContentRequirementHelper::stringToEnum(correctSlotsConfiguration[i].get<std::string>()));
        }
    }

    ConfigurationNode attributeParams = config["attributeRandomizationGuidelines"];
    m_commonData->attributeRandomizer.loadFromConfiguration(attributeParams);

    m_commonData->displayedName = config["displayedName"].getDefault<std::string>("");
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
        m_commonData->projectile = ResourceManager<ProjectilePrefab>::instance().get(config["projectile"].get<std::string>());
    }
}

bool WeaponTileModel::equals(const TileModel& other) const
{
    const WeaponTileModel& otherModel = static_cast<const WeaponTileModel&>(other);

    return m_attributes == otherModel.m_attributes;

    return true;
}
bool WeaponTileModel::isMovableFrom() const
{
    return true;
}
bool WeaponTileModel::isThrowableThrough() const
{
    return true;
}
bool WeaponTileModel::isMovableTo() const
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
bool WeaponTileModel::meetsRequirements(SlotContentRequirement req) const
{
    return m_commonData->validSlots.count(req) > 0;
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
    m_attributes = m_commonData->attributeRandomizer.randomize();
}

WeaponTileModel::AttackResult WeaponTileModel::attack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{
    AttackResult attackResult{ 0, 0 };

    if (m_commonData->requiresAmmo)
    {
        TileStack& ammo = player.ammo();
        if (ammo.isEmpty()) return attackResult;
        if (ammo.quantity() < m_commonData->ammoPerAttack) return attackResult;
        if (std::find(m_commonData->allowedAmmoGroups.begin(), m_commonData->allowedAmmoGroups.end(), ammo.tile().model().ammoGroup()) == m_commonData->allowedAmmoGroups.end()) return attackResult;

        ammo.tile().model().indirectAttack(world, player, hintedPosition);
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
