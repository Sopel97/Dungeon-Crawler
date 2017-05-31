#include "tiles/controllers/ContainerTileController.h"

#include "EventDispatcher.h"

#include "TileLocation.h"
#include "InventorySlotView.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

#include "Player.h"

ContainerTileController::ContainerTileController() :
    TileController()
{

}
ContainerTileController::ContainerTileController(const ContainerTileController& other) :
    TileController(other)
{
}
ContainerTileController::~ContainerTileController()
{
}

void ContainerTileController::use(Player& player, const TileLocation& location)
{
    player.tryInteractWithExternalInventory(owner(), *(m_owner->model().inventory()), location);
}

void ContainerTileController::use(Player& player, const InventorySlotView& slot)
{
    player.tryInteractWithInternalInventory(owner(), *(m_owner->model().inventory()), slot);
}

std::unique_ptr<TileController> ContainerTileController::clone() const
{
    return std::make_unique<ContainerTileController>(*this);
}