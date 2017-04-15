#include "..\..\include\tiles\TilePrefab.h"

#include "tiles/controllers/TileController.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/Tile.h"

int TilePrefab::m_lastId = -1;

TilePrefab::TilePrefab(std::unique_ptr<TileModel> model, std::unique_ptr<TileRenderer> renderer, std::unique_ptr<TileController> controller)
{
    m_modelCommonData = model->createCommonDataStorage();
    m_rendererCommonData = renderer->createCommonDataStorage();
    m_controllerCommonData = controller->createCommonDataStorage();

    model->setCommonDataStorage(*m_modelCommonData);
	renderer->setCommonDataStorage(*m_rendererCommonData);
    controller->setCommonDataStorage(*m_controllerCommonData);

    m_tile = std::make_unique<Tile>(++m_lastId, std::move(model), std::move(renderer), std::move(controller));
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
    return m_tile->clone();
}
