#pragma once

#include "Configuration.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class TileModel;
class TileView;
class TileController;
class TileLocation;
class Tile;
class ComponentCommonData;

class TilePrefab
{
public:
    TilePrefab(std::unique_ptr<TileModel> model, std::unique_ptr<TileView> view, std::unique_ptr<TileController> controller);
    ~TilePrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    std::unique_ptr<Tile> instantiate() const;
protected:
    std::unique_ptr<ComponentCommonData> m_modelCommonData;
    std::unique_ptr<ComponentCommonData> m_viewCommonData;
    std::unique_ptr<ComponentCommonData> m_controllerCommonData;

    std::unique_ptr<Tile> m_tile;

    static int m_lastId;
};

