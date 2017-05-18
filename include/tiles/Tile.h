#ifndef TILE_H
#define TILE_H

#include "Configuration.h"

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

class Tile
{
public:
    Tile(int id, std::unique_ptr<TileModel>&& model, std::unique_ptr<TileRenderer>&& renderer, std::unique_ptr<TileController>&& controller);
    Tile(const Tile& other);
    ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const TileModel& model() const;
    TileModel& model();
    const TileRenderer& renderer() const;
    TileRenderer& renderer();
    const TileController& controller() const;
    TileController& controller();

    bool equals(const Tile& other) const;

    int maxQuantity() const;

    void onTilePlaced(const TileLocation& location);
    void onTileRemoved(const TileLocation& location);
    void onTileInstantiated();
    void onTileCloned();

    int id() const;

    std::unique_ptr<Tile> clone() const;
    std::unique_ptr<Tile> instantiate() const;

    void onTileQuantityChanged(int oldQuantity, int newQuantity);
protected:
    std::unique_ptr<TileModel> m_model;
    std::unique_ptr<TileRenderer> m_renderer;
    std::unique_ptr<TileController> m_controller;
    int m_id; //NOTE: if memory is an issue these 3 can be changed to other types
    int m_maxQuantity;
};

#endif // TILE_H
