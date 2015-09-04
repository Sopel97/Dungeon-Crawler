#include "ContainerTileController.h"

#include "AttemptToInteractWithExternalInventory.h"
#include "EventDispatcher.h"

#include "TileLocation.h"

#include "Tile.h"
#include "TileModel.h"

ContainerTileController::ContainerTileController(Tile* owner) :
    TileController(owner)
{

}
ContainerTileController::ContainerTileController(const ContainerTileController& other) :
    TileController(other)
{
}
ContainerTileController::~ContainerTileController()
{
}

void ContainerTileController::onTileUsedByPlayer(const TileLocation& location)
{
    EventDispatcher::instance().broadcast<AttemptToInteractWithExternalInventory>(m_owner->model().inventory(), location.x, location.y);
}

std::unique_ptr<TileController> ContainerTileController::clone() const
{
    return std::make_unique<ContainerTileController>(*this);
}
std::unique_ptr<TileController> ContainerTileController::create(Tile* owner) const
{
    return std::make_unique<ContainerTileController>(owner);
}