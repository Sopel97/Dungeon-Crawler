#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

#include "TileLocation.h"

#include "ComponentCommonData.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

Tile::Tile(int id,
    const ComponentFactory<Tile, TileModel>& modelFac,
    const ComponentFactory<Tile, TileRenderer>& rendererFac,
    const ComponentFactory<Tile, TileController>& controllerFac) :
    m_model(modelFac.create(*this, modelFac.createCommonDataStorage())),
    m_renderer(rendererFac.create(*this, rendererFac.createCommonDataStorage())),
    m_controller(controllerFac.create(*this, controllerFac.createCommonDataStorage())),
    m_id(id)
{
}
Tile::Tile() :
    m_model(std::make_unique<TileModel>(*this)),
    m_renderer(std::make_unique<TileRenderer>(*this)),
    m_controller(std::make_unique<TileController>(*this)),
    m_id(-99)
{
}
Tile::Tile(const Tile& other) :
    m_model(other.m_model->clone(*this)),
    m_renderer(other.m_renderer->clone(*this)),
    m_controller(other.m_controller->clone(*this)),
    m_id(other.m_id)
{
}
Tile::Tile(Tile&& other) :
    m_model(std::move(other.m_model)),
    m_renderer(std::move(other.m_renderer)),
    m_controller(std::move(other.m_controller)),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);
}
Tile& Tile::operator=(const Tile& other)
{
    m_model = other.m_model->clone(*this);
    m_renderer = other.m_renderer->clone(*this);
    m_controller = other.m_controller->clone(*this);
    m_id = other.m_id;

    return *this;
}
Tile& Tile::operator=(Tile&& other)
{
    m_model = std::move(other.m_model);
    m_renderer = std::move(other.m_renderer);
    m_controller = std::move(other.m_controller);
    m_id = other.m_id;

    m_model->setOwner(this);
    m_renderer->setOwner(this);
    m_controller->setOwner(this);

    return *this;
}
Tile::~Tile()
{

}

void Tile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
    m_controller->loadFromConfiguration(config);
}

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_renderer->draw(renderTarget, renderStates, location);
}
void Tile::drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_renderer->drawOutside(renderTarget, renderStates, location);
}
void Tile::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const
{
    m_renderer->drawMeta(renderTarget, renderStates, location);
}

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const
{
    m_renderer->draw(renderTarget, renderStates, slot);
}

const TileModel& Tile::model() const
{
    return *m_model;
}
TileModel& Tile::model()
{
    return *m_model;
}
const TileRenderer& Tile::renderer() const
{
    return *m_renderer;
}
TileRenderer& Tile::renderer()
{
    return *m_renderer;
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
    return m_model->maxQuantity();
}

void Tile::onTileQuantityChanged(int oldQuantity, int newQuantity)
{
    m_model->onTileQuantityChanged(oldQuantity, newQuantity);
    m_renderer->onTileQuantityChanged(oldQuantity, newQuantity);
    m_controller->onTileQuantityChanged(oldQuantity, newQuantity);
}
void Tile::onTileInstantiated()
{
    m_model->onTileInstantiated();
    m_renderer->onTileInstantiated();
    m_controller->onTileInstantiated();
}
void Tile::onTileCloned()
{
    m_model->onTileCloned();
    m_renderer->onTileCloned();
    m_controller->onTileCloned();
}


int Tile::id() const
{
    return m_id;
}

std::unique_ptr<Tile> Tile::clone() const
{
    std::unique_ptr<Tile> tileClone = std::make_unique<Tile>(*this);

    tileClone->onTileCloned();

    return tileClone;
}
std::unique_ptr<Tile> Tile::instantiate() const
{
    std::unique_ptr<Tile> tileClone = std::make_unique<Tile>(*this);

    tileClone->onTileInstantiated();

    return tileClone;
}
