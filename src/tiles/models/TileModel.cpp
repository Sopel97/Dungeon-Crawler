#include "tiles/models/TileModel.h"

#include "AttributeSet.h"

#include "tiles/TileInformation.h"

#include "Player.h"

#include "TileDescriptionGenerator.h"

using namespace ls;

TileModel::TileModel(Tile& owner) :
    TileComponent(owner)
{

}
TileModel::TileModel(const TileModel& other, Tile& owner) :
    TileComponent(other, owner)
{

}
TileModel::~TileModel()
{

}

bool TileModel::equals(const TileModel& other) const
{
    return true;
}

std::optional<TileCollider> TileModel::collider(const ls::Vec2I& pos)
{
    return std::nullopt;
}
bool TileModel::isMovableFrom() const
{
    return false;
}
bool TileModel::isThrowableThrough() const
{
    return false;
}
bool TileModel::isMovableTo() const
{
    return false;
}
int TileModel::maxThrowDistance() const
{
    return 0;
}
bool TileModel::canBeStored() const
{
    return false;
}
int TileModel::maxQuantity() const
{
    return 1;
}
bool TileModel::meetsRequirements(SlotContentRequirement req) const
{
    return req == SlotContentRequirement::None
        || req == SlotContentRequirement::LeftHand
        || req == SlotContentRequirement::RightHand;
}
const std::string& TileModel::displayedName() const
{
    static const std::string name = "";
    return name;
}
const AttributeSet& TileModel::attributes() const
{
    static AttributeSet empty;
    return empty;
}
TileInformation TileModel::additionalInformation() const
{
    return {};
}

Inventory* TileModel::inventory()
{
    return nullptr;
}
const Inventory* TileModel::inventory() const
{
    return nullptr;
}

float TileModel::drag() const
{
    return 1.0f;
}

void TileModel::use(Player& player, const TileLocation& location)
{

}
void TileModel::use(Player& player, const InventorySlotView& location)
{

}
void TileModel::look(Player& player, const TileLocation& location)
{
    player.showTileDescription(TileDescriptionGenerator::generate(owner()));
}
void TileModel::look(Player& player, const InventorySlotView& location)
{
    player.showTileDescription(TileDescriptionGenerator::generate(owner()));
}
TileModel::AttackResult TileModel::attack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{
    return { 0, 0 };
}
void TileModel::indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{

}
TileAmmoGroupType TileModel::ammoGroup() const
{
    return TileAmmoGroupType();
}

std::unique_ptr<TileModel> TileModel::clone(Tile& owner) const
{
    return std::make_unique<TileModel>(*this, owner);
}
