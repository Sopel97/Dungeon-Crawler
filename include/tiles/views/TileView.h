#ifndef TILEVIEW_H
#define TILEVIEW_H

#include "Configuration.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class Tile;

class TileView
{
public:
    TileView(Tile* owner);
    TileView(const TileView& other);
    virtual ~TileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map) = 0;

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual std::unique_ptr<TileView> clone() const = 0;
protected:
    Tile* m_owner;
};

#endif // TILEVIEW_H
