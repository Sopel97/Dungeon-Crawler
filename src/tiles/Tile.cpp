#include "Tile.h"

#include "TileView.h"
#include "TileModel.h"
#include "TileController.h"
#include "TileView.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

Tile::Tile(std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller) :
    m_model(std::move(model)),
    m_view(std::move(view)),
    m_controller(std::move(controller))
{

}
Tile::~Tile()
{

}

void Tile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_view->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);
}

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map)
{
    m_view->draw(renderTarget, renderStates, x, y, map);
}

const std::unique_ptr<TileModel>& Tile::model() const
{
    return m_model;
}
const std::unique_ptr<TileView>& Tile::view() const
{
    return m_view;
}
const std::unique_ptr<TileController>& Tile::controller() const
{
    return m_controller;
}

