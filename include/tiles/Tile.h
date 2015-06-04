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
class TileView;
class TileController;

class Tile
{
public:
    Tile(std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller);
    Tile(const Tile& other);
    virtual ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const;

    const TileModel& model() const;
    const TileView& view() const;
    const TileController& controller() const;

    int id() const;

    virtual std::unique_ptr<Tile> clone() const;
protected:
    std::unique_ptr<TileModel> m_model;
    std::unique_ptr<TileView> m_view;
    std::unique_ptr<TileController> m_controller;
    int m_id;

    static int m_lastId;
};

#endif // TILE_H
