#include "PlainTileController.h"

#include "Tile.h"

#include "make_unique.h"

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
