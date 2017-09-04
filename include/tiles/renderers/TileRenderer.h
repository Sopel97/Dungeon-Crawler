#pragma once

#include "configuration/Configuration.h"

#include "DynamicEnum.h"

#include "ComponentCommonData.h"
#include "tiles/TileComponent.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class Tile;
class TileLocation;
class InventorySlotView;
class SpriteBatch;
struct TileOuterBorderCache;

struct InnerBorderGroup;
using TileInnerBorderGroupType = RuntimeIdentifier<InnerBorderGroup>;

class TileRenderer : public TileComponent<TileRenderer, Tile>
	//must be functional (ie. all methods must return resonable values and there is no pure virtual member functions)
{
public:
    TileRenderer(Tile& owner);
    TileRenderer(const TileRenderer& other, Tile& owner);
    ~TileRenderer() override;

    virtual void draw(SpriteBatch& mainSriteBatch, SpriteBatch& metaSpriteBatch, const TileLocation& location) const;

    virtual void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const InventorySlotView& slot) const;
    virtual void drawOutside(SpriteBatch& spriteBatch, const TileLocation& location, const TileOuterBorderCache& cache) const;

    virtual TileOuterBorderCache buildOuterBorderCache(const TileLocation& location) const;

    virtual TileInnerBorderGroupType innerBorderGroup() const;
    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
};
