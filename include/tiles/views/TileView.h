#ifndef TILEVIEW_H
#define TILEVIEW_H

#include "Configuration.h"

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
    virtual ~TileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map) = 0;

    const Tile* owner() const;
protected:
    Tile* m_owner;
};

#endif // TILEVIEW_H
