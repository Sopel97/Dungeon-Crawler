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

class PlainTileView : public TileView
{
public:
    PlainTileView(Tile* owner);
    PlainTileView(const PlainTileView& other);
    virtual ~PlainTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const;

    const sf::Texture& texture() const;
    const Geo::Vec2I& currentSprite() const;

    int selectRandomSprite() const;

    virtual std::unique_ptr<TileView> clone() const;
protected:
    struct CommonData
    {
        ResourceHandle<sf::Texture> texture;
        std::vector<Geo::Vec2I> sprites;
        std::vector<float> spritesWeightsSums;
    };
    std::shared_ptr<CommonData> m_commonData;
    int m_currentSprite;
};

REGISTER_TILE_VIEW_TYPE(PlainTileView)

#endif // PLAINTILEVIEW_H
