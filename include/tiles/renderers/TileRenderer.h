#pragma once

#include "Configuration.h"

#include "../../ComponentCommonData.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class Tile;
class TileLocation;

class TileRenderer //must be functions (ie. all methods must return resonable values and there is no pure virtual member functions)
{
public:
    TileRenderer();
    TileRenderer(const TileRenderer& other);
    virtual ~TileRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    void setOwner(Tile* newOwner);

    virtual int innerBorderGroup() const;
    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual bool isTall() const; //tall tiles are required to have colliders

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileRenderer> clone() const;
protected:
    Tile* m_owner;
};
