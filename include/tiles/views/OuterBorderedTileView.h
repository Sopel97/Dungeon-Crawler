#ifndef OUTERBORDEREDTILEVIEW_H
#define OUTERBORDEREDTILEVIEW_H

#include "TileView.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "WeightedSpriteSet.h"

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

class OuterBorderedTileView : public TileView
{
public:
    OuterBorderedTileView();
    OuterBorderedTileView(const OuterBorderedTileView& other);
    virtual ~OuterBorderedTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;

    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileView> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        WeightedSpriteSet spriteSet;
        ls::Vec2I borderSprites;

        int outerBorderPriority;
    };
    CommonData* m_commonData;
    ls::Vec2I m_sprite;
};

REGISTER_TILE_VIEW_TYPE(OuterBorderedTileView)

#endif // OUTERBORDEREDTILEVIEW_H
