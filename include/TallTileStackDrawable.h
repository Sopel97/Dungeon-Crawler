#ifndef TALLTILESTACKDRAWABLE_H
#define TALLTILESTACKDRAWABLE_H

#include "TallDrawable.h"

#include "../LibS/GeometryLight.h"

class TileStack;
class MapLayer;

class TallTileStackDrawable : public TallDrawable //requires at least one tile in tile stack to be tall
{
public:
    TallTileStackDrawable(const TileStack& tileStack, int x, int y, const MapLayer& map);
    virtual ~TallTileStackDrawable();

    virtual const Geo::RectangleF& boundingRectangle() const;
    virtual const Geo::Vec2F& center() const;

    virtual TallDrawable::DrawableType type() const;

    int tileX() const;
    int tileY() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    const TileStack& m_tileStack;
    int m_tileX;
    int m_tileY;
    const MapLayer& m_map;

    Geo::RectangleF m_boundingRectangle;
    Geo::Vec2F m_center;
    int m_indexOfFirstTallTile;
};

#endif // TALLTILESTACKDRAWABLE_H
