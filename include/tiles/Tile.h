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
class TileLocation;

class Tile
{
public:
    Tile(std::unique_ptr<TileModel>&& model, std::unique_ptr<TileView>&& view, std::unique_ptr<TileController>&& controller);
    Tile(const Tile& other);
    virtual ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const TileModel& model() const;
    TileModel& model();
    const TileView& view() const;
    TileView& view();
    const TileController& controller() const;
    TileController& controller();

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
