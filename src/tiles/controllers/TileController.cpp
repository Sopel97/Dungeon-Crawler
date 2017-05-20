#include "tiles/controllers/TileController.h"

TileController::TileController() :
    Component()
{

}
TileController::TileController(const TileController& other) :
	Component(other)
{

}
TileController::~TileController()
{

}

void TileController::onTilePlaced(const TileLocation& location)
{

}
void TileController::onTileRemoved(const TileLocation& location)
{

}
void TileController::onTileQuantityChanged(int oldQuantity, int newQuantity)
{

}
void TileController::onTileInteracted(Player& player, const TileLocation& location)
{

}
void TileController::onTileInteracted(Player& player, const InventorySlotView& location)
{

}
void TileController::onTileInstantiated()
{

}
void TileController::onTileCloned()
{

}

std::unique_ptr<TileController> TileController::clone() const
{
    return std::make_unique<TileController>(*this);
}
