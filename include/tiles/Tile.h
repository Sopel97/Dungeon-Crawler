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
    ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const TileModel& model() const;
    TileModel& model();
    const TileView& view() const;
    TileView& view();
    const TileController& controller() const;
    TileController& controller();

    bool equals(const Tile& other) const;

    int quantity() const;
    void setQuantity(int newQuantity);
    int maxQuantity() const;

    void onTilePlaced(const TileLocation& location);
    void onTileRemoved(const TileLocation& location);

    int id() const;

    std::unique_ptr<Tile> clone() const;
protected:
    std::unique_ptr<TileModel> m_model;
    std::unique_ptr<TileView> m_view;
    std::unique_ptr<TileController> m_controller;
    int m_id; //NOTE: if memory is an issue these 3 can be changed to other types
    int m_quantity;
    int m_maxQuantity;

    static int m_lastId;
};

#endif // TILE_H
