#include "tiles/controllers/ContainerTileController.h"

#include "EventDispatcher.h"

#include "TileLocation.h"
#include "InventorySlotView.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

#include "Player.h"

REGISTER_TILE_CONTROLLER_TYPE(ContainerTileController)

ContainerTileController::ContainerTileController(Tile& owner) :
    TileController(owner)
{

}
ContainerTileController::ContainerTileController(const ContainerTileController& other, Tile& owner) :
    TileController(other, owner)
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

std::unique_ptr<TileController> ContainerTileController::clone(Tile& owner) const
{
    return std::make_unique<ContainerTileController>(*this, owner);
}