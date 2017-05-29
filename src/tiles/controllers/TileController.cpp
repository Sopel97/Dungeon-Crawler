#include "tiles/controllers/TileController.h"

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
std::unique_ptr<TileController> TileController::clone() const
{
    return std::make_unique<TileController>(*this);
}
