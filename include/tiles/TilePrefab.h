#pragma once

#include "Configuration.h"

#include "ComponentFactory.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class TileModel;
class TileRenderer;
class TileController;
class TileLocation;
class Tile;
class ComponentCommonData;

class TilePrefab
{
public:
    TilePrefab(
        const ComponentFactory<Tile, TileModel>& modelFac, 
        const ComponentFactory<Tile, TileRenderer>& rendererFac, 
        const ComponentFactory<Tile, TileController>& controllerFac);
    ~TilePrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    std::unique_ptr<Tile> instantiate() const;
protected:
    std::unique_ptr<Tile> m_tile;

    const ComponentFactory<Tile, TileModel>* m_modelFactory;
    const ComponentFactory<Tile, TileRenderer>* m_rendererFactory;
    const ComponentFactory<Tile, TileController>* m_controllerFactory;

    static int m_lastId;
};

