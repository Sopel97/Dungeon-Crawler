#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/views/TileView.h"
#include "tiles/controllers/TileController.h"

#include "TileLocation.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

Tile::Tile(int id, std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller) :
    m_model(std::move(model)),
    m_view(std::move(view)),
    m_controller(std::move(controller)),
    m_id(id),
    m_maxQuantity(1)
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Tile::Tile(const Tile& other) :
    m_model(other.m_model->clone()),
    m_view(other.m_view->clone()),
    m_controller(other.m_controller->clone()),
    m_id(other.m_id),
    m_maxQuantity(other.m_maxQuantity)
{
    m_model->setOwner(this);
    m_view->setOwner(this);
    m_controller->setOwner(this);
}
Tile::~Tile()
{

}

void Tile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_view->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);

    m_maxQuantity = config["maxQuantity"].getDefault<int>(1);
}

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_view->draw(renderTarget, renderStates, location);
}
void Tile::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_view->drawOutside(renderTarget, renderStates, location);
}
void Tile::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_view->drawMeta(renderTarget, renderStates, location);
}

const TileModel& Tile::model() const
{
    return *m_model;
}
TileModel& Tile::model()
{
    return *m_model;
}
const TileView& Tile::view() const
{
    return *m_view;
}
TileView& Tile::view()
{
    return *m_view;
}
const TileController& Tile::controller() const
{
    return *m_controller;
}
TileController& Tile::controller()
{
    return *m_controller;
}

bool Tile::equals(const Tile& other) const
{
    return m_id == other.m_id && m_model->equals(*(other.m_model));
}

int Tile::maxQuantity() const
{
    return m_maxQuantity;
}
void Tile::onTilePlaced(const TileLocation& location)
{
    m_model->onTilePlaced(location);
    m_view->onTilePlaced(location);
    m_controller->onTilePlaced(location);
}
void Tile::onTileRemoved(const TileLocation& location)
{
    m_model->onTileRemoved(location);
    m_view->onTileRemoved(location);
    m_controller->onTileRemoved(location);
}
void Tile::onTileInstantiated()
{
    m_model->onTileInstantiated();
    m_view->onTileInstantiated();
    m_controller->onTileInstantiated();
}


int Tile::id() const
{
    return m_id;
}

std::unique_ptr<Tile> Tile::clone() const
{
    std::unique_ptr<Tile> tileClone = std::make_unique<Tile>(*this);

    tileClone->onTileInstantiated();

    return tileClone;
}

void Tile::onTileQuantityChanged(int oldQuantity, int newQuantity)
{
    m_model->onTileQuantityChanged(oldQuantity, newQuantity);
    m_view->onTileQuantityChanged(oldQuantity, newQuantity);
    m_controller->onTileQuantityChanged(oldQuantity, newQuantity);
}
