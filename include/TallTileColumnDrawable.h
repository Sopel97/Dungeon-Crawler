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
    TallTileColumnDrawable(TileColumn & tileColumn, const TileLocation & tileLocation);
    ~TallTileColumnDrawable() override;

    const ls::Rectangle2F& boundingRectangle() const override;
    const ls::Vec2F& center() const override;

    TallDrawable::DrawableType type() const override;

    int tileX() const;
    int tileY() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;

protected:
    TileColumn& m_tileColumn;
    int m_tileX;
    int m_tileY;
    const MapLayer& m_map;

    ls::Rectangle2F m_boundingRectangle;
    ls::Vec2F m_center;
    int m_indexOfFirstTallTile;
};

#endif // TALLTILESTACKDRAWABLE_H
