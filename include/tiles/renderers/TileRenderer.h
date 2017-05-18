#pragma once

#include "Configuration.h"

#include "ComponentCommonData.h"
#include "Component.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class Tile;
class TileLocation;

class TileRenderer : public Component<TileRenderer, Tile>
	//must be functional (ie. all methods must return resonable values and there is no pure virtual member functions)
{
public:
    TileRenderer();
    TileRenderer(const TileRenderer& other);
    virtual ~TileRenderer();

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual int innerBorderGroup() const;
    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual bool isTall() const; //tall tiles are required to have colliders

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInstantiated();
    virtual void onTileCloned();

    virtual std::unique_ptr<TileRenderer> clone() const;
};
