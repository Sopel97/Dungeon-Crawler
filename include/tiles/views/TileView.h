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

class TileView //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileView(Tile* owner);
    TileView(const TileView& other);
    virtual ~TileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const;

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual int innerBorderGroup() const;
    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual std::unique_ptr<TileView> clone() const;
    virtual std::unique_ptr<TileView> create(Tile* owner) const;
protected:
    Tile* m_owner;

    static constexpr int tileSize = 32; //TODO: think about what this should belong to
};

#endif // TILEVIEW_H
