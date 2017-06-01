#include "tiles/controllers/TileController.h"

#include "Player.h"
#include "TileDescriptionGenerator.h"

TileController::TileController() :
    TileComponent()
{

}
TileController::TileController(const TileController& other) :
    TileComponent(other)
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
std::unique_ptr<TileController> TileController::clone() const
{
    return std::make_unique<TileController>(*this);
}
