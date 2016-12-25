#ifndef PLAINTILEVIEW_H
#define PLAINTILEVIEW_H

#include "TileView.h"

#include "WeightedSpriteSet.h"

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

class PlainTileView : public TileView
{
public:
    PlainTileView();
    PlainTileView(const PlainTileView& other);
    virtual ~PlainTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;

    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileView> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        WeightedSpriteSet spriteSet;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    CommonData* m_commonData;
    ls::Vec2I m_sprite;
};

REGISTER_TILE_VIEW_TYPE(PlainTileView)

#endif // PLAINTILEVIEW_H
