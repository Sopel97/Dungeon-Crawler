#include "TileController.h"


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
