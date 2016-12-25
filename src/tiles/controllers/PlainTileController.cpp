#include "tiles/controllers/PlainTileController.h"

#include "tiles/Tile.h"

PlainTileController::PlainTileController() :
    TileController()
{

}

PlainTileController::PlainTileController(const PlainTileController& other) :
    TileController(other)
{

}

PlainTileController::~PlainTileController()
{

}


std::unique_ptr<TileController> PlainTileController::clone() const
{
    return std::make_unique<PlainTileController>(*this);
}

