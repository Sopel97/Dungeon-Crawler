#include "Tile.h"

#include "TileModel.h"
#include "TileView.h"
#include "TileController.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/make_unique.h"

int Tile::m_lastId = -1;

Tile::Tile(std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller) :
    m_model(std::move(model)),
    m_view(std::move(view)),
    m_controller(std::move(controller)),
    m_id(++m_lastId)
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Tile::Tile(const Tile& other) :
    m_model(other.m_model->clone()),
    m_view(other.m_view->clone()),
    m_controller(other.m_controller->clone()),
    m_id(other.m_id)
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

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
{
    m_view->draw(renderTarget, renderStates, x, y, z, map);
}
void Tile::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const
{
    m_view->drawOutside(renderTarget, renderStates, x, y, z, map);
}

const TileModel& Tile::model() const
{
    return *m_model;
}
const TileView& Tile::view() const
{
    return *m_view;
}
const TileController& Tile::controller() const
{
    return *m_controller;
}

int Tile::id() const
{
    return m_id;
}

std::unique_ptr<Tile> Tile::clone() const
{
    return std::make_unique<Tile>(*this);
}
