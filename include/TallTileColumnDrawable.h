#ifndef TALLTILESTACKDRAWABLE_H
#define TALLTILESTACKDRAWABLE_H

#include "TallDrawable.h"

#include "TileLocation.h"

#include "../LibS/Geometry.h"

class TileColumn;
class MapLayer;

class TallTileColumnDrawable : public TallDrawable //requires at least one tile in tile stack to be tall
{
public:
    TallTileColumnDrawable(const TileColumn & tileColumn, const TileLocation & tileLocation);
    virtual ~TallTileColumnDrawable();

    virtual const ls::Rectangle2F& boundingRectangle() const;
    virtual const ls::Vec2F& center() const;

    virtual TallDrawable::DrawableType type() const;

    int tileX() const;
    int tileY() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    const TileColumn& m_tileColumn;
    int m_tileX;
    int m_tileY;
    const MapLayer& m_map;

    ls::Rectangle2F m_boundingRectangle;
    ls::Vec2F m_center;
    int m_indexOfFirstTallTile;
};

#endif // TALLTILESTACKDRAWABLE_H
