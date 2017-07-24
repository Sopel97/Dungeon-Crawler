#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"

#include "TileLocation.h"

#include "ComponentCommonData.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

Tile::Tile(int id,
    const ComponentFactory<Tile, TileModel>& modelFac,
    const ComponentFactory<Tile, TileRenderer>& rendererFac) :
    m_model(modelFac.createWithNewCommonData(*this)),
    m_renderer(rendererFac.createWithNewCommonData(*this)),
    m_id(id)
{
}
Tile::Tile() :
    m_model(std::make_unique<TileModel>(*this)),
    m_renderer(std::make_unique<TileRenderer>(*this)),
    m_id(-99)
{
}
Tile::Tile(const Tile& other) :
    m_model(other.m_model->clone(*this)),
    m_renderer(other.m_renderer->clone(*this)),
    m_id(other.m_id)
{
}
Tile::Tile(Tile&& other) :
    m_model(std::move(other.m_model)),
    m_renderer(std::move(other.m_renderer)),
    m_id(other.m_id)
{
    m_model->setOwner(this);
    m_renderer->setOwner(this);
}
Tile& Tile::operator=(const Tile& other)
{
    m_model = other.m_model->clone(*this);
    m_renderer = other.m_renderer->clone(*this);
    m_id = other.m_id;

    return *this;
}
Tile& Tile::operator=(Tile&& other)
{
    m_model = std::move(other.m_model);
    m_renderer = std::move(other.m_renderer);
    m_id = other.m_id;

    m_model->setOwner(this);
    m_renderer->setOwner(this);

    return *this;
}
Tile::~Tile()
{

}

void Tile::loadFromConfiguration(ConfigurationNode& config)
{
    m_model->loadFromConfiguration(config);
    m_renderer->loadFromConfiguration(config);
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
}
void Tile::onTileInstantiated()
{
    m_model->onTileInstantiated();
    m_renderer->onTileInstantiated();
}
void Tile::onTileCloned()
{
    m_model->onTileCloned();
    m_renderer->onTileCloned();
}
void Tile::onTilePlaced(const TileLocation& location)
{
    m_model->onTilePlaced(location);
    m_renderer->onTilePlaced(location);
}
void Tile::onTilePlaced(const Inventory& inventory, int slot)
{
    m_model->onTilePlaced(inventory, slot);
    m_renderer->onTilePlaced(inventory, slot);
}
void Tile::onTileRemoved(const TileLocation& location)
{
    m_model->onTileRemoved(location);
    m_renderer->onTileRemoved(location);
}
void Tile::onTileRemoved(const Inventory& inventory, int slot)
{
    m_model->onTileRemoved(inventory, slot);
    m_renderer->onTileRemoved(inventory, slot);
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
