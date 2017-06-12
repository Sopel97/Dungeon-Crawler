#include "..\..\include\tiles\TilePrefab.h"

#include "tiles/controllers/TileController.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/Tile.h"

int TilePrefab::m_lastId = -1;

TilePrefab::TilePrefab(
    const ComponentFactory<Tile, TileModel>& modelFac, 
    const ComponentFactory<Tile, TileRenderer>& rendererFac, 
    const ComponentFactory<Tile, TileController>& controllerFac) :
    m_tile(std::make_unique<Tile>(++m_lastId, modelFac, rendererFac, controllerFac)),
    m_modelFactory(&modelFac),
    m_rendererFactory(&rendererFac),
    m_controllerFactory(&controllerFac)
{
}

TilePrefab::~TilePrefab()
{
}

void TilePrefab::loadFromConfiguration(ConfigurationNode& config)
{
    m_tile->loadFromConfiguration(config);
}

std::unique_ptr<Tile> TilePrefab::instantiate() const
{
    return m_tile->instantiate();
}
