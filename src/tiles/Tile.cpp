#include "Tile.h"

#include "TileView.h"
#include "TileModel.h"
#include "TileController.h"
#include "TileView.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/make_unique.h"

Tile::Tile(std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller) :
    m_model(std::move(model)),
    m_view(std::move(view)),
    m_controller(std::move(controller))
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Tile::Tile(const Tile& other) :
    m_model(other.m_model->clone()),
    m_view(other.m_view->clone()),
    m_controller(other.m_controller->clone())
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

std::unique_ptr<Tile> Tile::clone() const
{
    return std::make_unique<Tile>(*this);
}
