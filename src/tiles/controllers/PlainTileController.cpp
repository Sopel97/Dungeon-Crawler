#include "tiles/controllers/PlainTileController.h"

#include "tiles/Tile.h"

REGISTER_TILE_CONTROLLER_TYPE(PlainTileController)

PlainTileController::PlainTileController(Tile& owner) :
    TileController(owner)
{

}

PlainTileController::PlainTileController(const PlainTileController& other, Tile& owner) :
    TileController(other, owner)
{

}

PlainTileController::~PlainTileController()
{

}


std::unique_ptr<TileController> PlainTileController::clone(Tile& owner) const
{
    return std::make_unique<PlainTileController>(*this, owner);
}

