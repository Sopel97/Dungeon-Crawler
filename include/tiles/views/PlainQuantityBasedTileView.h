#ifndef PLAINQUANTITYBASEDTILEVIEW_H
#define PLAINQUANTITYBASEDTILEVIEW_H

#include "TileView.h"

#include "QuantityBasedSpriteSet.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include <vector>

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class MapLayer;
class Tile;
class TileLocation;

class PlainQuantityBasedTileView : public TileView
{
public:
    PlainQuantityBasedTileView(Tile* owner);
    PlainQuantityBasedTileView(const PlainQuantityBasedTileView& other);
    virtual ~PlainQuantityBasedTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;

    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileQuantityChanged(int newQuantity);

    virtual std::unique_ptr<TileView> clone() const;
    virtual std::unique_ptr<TileView> create(Tile* owner) const;
protected:
    struct CommonData
    {
        ResourceHandle<sf::Texture> texture;
        QuantityBasedSpriteSet spriteSet;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    std::shared_ptr<CommonData> m_commonData;
    ls::Vec2I m_sprite;
};

REGISTER_TILE_VIEW_TYPE(PlainQuantityBasedTileView)

#endif // PLAINQUANTITYBASEDTILEVIEW_H
