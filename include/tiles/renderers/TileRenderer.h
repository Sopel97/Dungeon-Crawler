#pragma once

#include "Configuration.h"

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

class TileRenderer : public TileComponent<TileRenderer, Tile>
	//must be functional (ie. all methods must return resonable values and there is no pure virtual member functions)
{
public:
    TileRenderer(Tile& owner);
    TileRenderer(const TileRenderer& other, Tile& owner);
    ~TileRenderer() override;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const;

    virtual int innerBorderGroup() const;
    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual bool isTall() const; //tall tiles are required to have colliders

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
};
