#include "tiles/controllers/TileController.h"

TileController::TileController() :
    m_owner(nullptr)
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
void TileController::onTileQuantityChanged(int oldQuantity, int newQuantity)
{

}
void TileController::onTileInteracted(Player& player, const TileLocation& location)
{

}
void TileController::onTileInstantiated()
{

}

std::unique_ptr<ComponentCommonData> TileController::createCommonDataStorage() const
{
    return nullptr;
}
void TileController::setCommonDataStorage(ComponentCommonData& commonData)
{

}

std::unique_ptr<TileController> TileController::clone() const
{
    return std::make_unique<TileController>(*this);
}
