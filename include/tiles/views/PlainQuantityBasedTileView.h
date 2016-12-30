#ifndef PLAINQUANTITYBASEDTILEVIEW_H
#define PLAINQUANTITYBASEDTILEVIEW_H

#include "TileView.h"

#include "QuantityBasedSpriteSet.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

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
    PlainQuantityBasedTileView();
    PlainQuantityBasedTileView(const PlainQuantityBasedTileView& other);
    virtual ~PlainQuantityBasedTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;

    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileView> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        QuantityBasedSpriteSet spriteSet;
        std::vector<ls::Vec2I> respectiveMetaSprites;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    CommonData* m_commonData;
    int m_spriteId;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const ls::Vec2I& sprite) const;
};

REGISTER_TILE_VIEW_TYPE(PlainQuantityBasedTileView)

#endif // PLAINQUANTITYBASEDTILEVIEW_H
