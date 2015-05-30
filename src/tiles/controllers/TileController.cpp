#include "TileController.h"


TileController::TileController(Tile* owner) :
    m_owner(owner)
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
