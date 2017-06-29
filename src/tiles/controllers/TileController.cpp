#include "tiles/controllers/TileController.h"

#include "Player.h"
#include "TileDescriptionGenerator.h"

TileController::TileController(Tile& owner) :
    TileComponent(owner)
{

}
TileController::TileController(const TileController& other, Tile& owner) :
    TileComponent(other, owner)
{

}
TileController::~TileController()
{

}

void TileController::use(Player& player, const TileLocation& location)
{

}
void TileController::use(Player& player, const InventorySlotView& location)
{

}
void TileController::look(Player& player, const TileLocation& location)
{
    player.showTileDescription(TileDescriptionGenerator::generate(owner()));
}
void TileController::look(Player& player, const InventorySlotView& location)
{
    player.showTileDescription(TileDescriptionGenerator::generate(owner()));
}
void TileController::attack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{

}
void TileController::indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition)
{

}
TileAmmoGroupType TileController::ammoGroup() const
{
    return TileAmmoGroupType();
}
std::unique_ptr<TileController> TileController::clone(Tile& owner) const
{
    return std::make_unique<TileController>(*this, owner);
}
