#include "PlainTileController.h"

#include "Tile.h"

PlainTileController::PlainTileController(Tile* owner) :
    TileController(owner)
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


std::unique_ptr<TileController> PlainTileController::create(Tile* owner) const
{
    return std::make_unique<PlainTileController>(owner);
}
