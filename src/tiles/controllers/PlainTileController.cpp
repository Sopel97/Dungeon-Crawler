#include "PlainTileController.h"

#include "Tile.h"

#include "../LibS/make_unique.h"

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

void PlainTileController::onTilePlaced(const TileLocation& location)
{

}
void PlainTileController::onTileRemoved(const TileLocation& location)
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
