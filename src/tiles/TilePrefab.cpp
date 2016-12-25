#include "..\..\include\tiles\TilePrefab.h"

#include "tiles/controllers/TileController.h"
#include "tiles/models/TileModel.h"
#include "tiles/views/TileView.h"
#include "tiles/Tile.h"

int TilePrefab::m_lastId = -1;

TilePrefab::TilePrefab(std::unique_ptr<TileModel> model, std::unique_ptr<TileView> view, std::unique_ptr<TileController> controller)
{
    m_modelCommonData = model->createCommonDataStorage();
    m_viewCommonData = view->createCommonDataStorage();
    m_controllerCommonData = controller->createCommonDataStorage();

    model->setCommonDataStorage(*m_modelCommonData);
    view->setCommonDataStorage(*m_viewCommonData);
    controller->setCommonDataStorage(*m_controllerCommonData);

    m_tile = std::make_unique<Tile>(++m_lastId, std::move(model), std::move(view), std::move(controller));
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
