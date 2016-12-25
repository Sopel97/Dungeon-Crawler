#include "tiles/controllers/ContainerTileController.h"

#include "events/AttemptToInteractWithExternalInventory.h"
#include "EventDispatcher.h"

#include "TileLocation.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

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

void ContainerTileController::onTileUsedByPlayer(const TileLocation& location)
{
    EventDispatcher::instance().broadcast<AttemptToInteractWithExternalInventory>(m_owner->model().inventory(), location.x, location.y);
}

std::unique_ptr<TileController> ContainerTileController::clone() const
{
    return std::make_unique<ContainerTileController>(*this);
}