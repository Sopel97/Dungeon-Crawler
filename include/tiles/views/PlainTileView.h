#ifndef PLAINTILEVIEW_H
#define PLAINTILEVIEW_H

#include "TileView.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "../LibS/GeometryLight.h"

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

class PlainTileView : public TileView
{
public:
    PlainTileView(Tile* owner);
    PlainTileView(const PlainTileView& other);
    virtual ~PlainTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;
    const Geo::Vec2I& selectedSprite() const;

    int selectRandomSprite() const;
    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);

    virtual std::unique_ptr<TileView> clone() const;
    virtual std::unique_ptr<TileView> create(Tile* owner) const;
protected:
    struct CommonData
    {
        ResourceHandle<sf::Texture> texture;
        std::vector<Geo::Vec2I> sprites;
        std::vector<float> spritesWeightsSums;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    std::shared_ptr<CommonData> m_commonData;
    int m_selectedSprite;
};

REGISTER_TILE_VIEW_TYPE(PlainTileView)

#endif // PLAINTILEVIEW_H
