#ifndef INNERBORDEREDWALLTILEVIEW_H
#define INNERBORDEREDWALLTILEVIEW_H


#include "TileView.h"

#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class MapLayer;
class Tile;

class InnerBorderedWallTileView : public TileView
{
public:
    InnerBorderedWallTileView(Tile* owner);
    InnerBorderedWallTileView(const InnerBorderedWallTileView& other);
    virtual ~InnerBorderedWallTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, int z, const MapLayer& map) const;

    const sf::Texture& texture() const;
    const Geo::Vec2I& spriteSet() const;
    virtual int innerBorderGroup() const;

    virtual std::unique_ptr<TileView> clone() const;
protected:
    struct CommonData
    {
        ResourceHandle<sf::Texture> texture;

        Geo::Vec2I spriteSet;

        //specific names are based on appearance in file "inner_bordered_wall_sprites_preview.png"
        Geo::Vec2I full;
        Geo::Vec2I top;
        Geo::Vec2I left;
        Geo::Vec2I topLeft;
        Geo::Vec2I bottomRight;

        Geo::Vec2I outerLeft;
        Geo::Vec2I innerTopRight;
        Geo::Vec2I innerBottomLeft;
        Geo::Vec2I outerTopRight;
        Geo::Vec2I outerBottomLeft;
        Geo::Vec2I outerTop;
        Geo::Vec2I outerTopLeft;
        Geo::Vec2I innerBottomRight;

        int innerBorderGroup;
    };
    std::shared_ptr<CommonData> m_commonData;
};

#endif // INNERBORDEREDWALLTILEVIEW_H
