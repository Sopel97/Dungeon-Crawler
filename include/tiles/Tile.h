#ifndef TILE_H
#define TILE_H

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
class InventorySlotView;
class ComponentCommonData;

class Tile
{
public:
    Tile(
        int id,
        const ComponentFactory<Tile, TileModel>& modelFac,
        const ComponentFactory<Tile, TileRenderer>& rendererFac,
        const ComponentFactory<Tile, TileController>& controllerFac);
    Tile(); // temporary for creating 'empty' tile, used to avoid checking if tile exists, tile column just returns this
    Tile(const Tile& other);
    Tile(Tile&& other);
    Tile& operator=(const Tile& other);
    Tile& operator=(Tile&& other);
    ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    const TileModel& model() const;
    TileModel& model();
    const TileRenderer& renderer() const;
    TileRenderer& renderer();
    const TileController& controller() const;
    TileController& controller();

    bool equals(const Tile& other) const;

    int maxQuantity() const;

    void onTileQuantityChanged(int oldQuantity, int newQuantity);
    void onTileInstantiated();
    void onTileCloned();

    int id() const;

    std::unique_ptr<Tile> clone() const;
    std::unique_ptr<Tile> instantiate() const;

protected:
    std::unique_ptr<TileModel> m_model;
    std::unique_ptr<TileRenderer> m_renderer;
    std::unique_ptr<TileController> m_controller;
    int m_id;
};

#endif // TILE_H
