#include "tiles/controllers/TileController.h"

TileController::TileController(Tile* owner) :
    m_owner(owner)
{

}
TileController::TileController(const TileController& other) :
    m_owner(other.m_owner)
{

}
TileController::~TileController()
{

}

void TileController::loadFromConfiguration(ConfigurationNode& config)
{

}

const Tile* TileController::owner() const
{
    return m_owner;
}

void TileController::setOwner(Tile* newOwner)
{
    m_owner = newOwner;
}

void TileController::onTilePlaced(const TileLocation& location)
{

}
void TileController::onTileRemoved(const TileLocation& location)
{

}
void TileController::onTileQuantityChanged(int newQuantity)
{

}
void TileController::onTileUsedByPlayer(const TileLocation& location)
{

}

std::unique_ptr<TileController> TileController::clone() const
{
    return std::make_unique<TileController>(*this);
}

std::unique_ptr<TileController> TileController::create(Tile* owner) const
{
    return std::make_unique<TileController>(owner);
}
