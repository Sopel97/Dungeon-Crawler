#pragma once

#include "Configuration.h"
#include "Component.h"
#include "ComponentCommonData.h"

#include <memory>

class Tile;
class TileLocation;
class Player;
class InventorySlotView;

class TileController : public Component<TileController, Tile>
	//must not be abstract (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileController();
    TileController(const TileController& other);
    virtual ~TileController();

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInteracted(Player& player, const TileLocation& location);
    virtual void onTileInteracted(Player& player, const InventorySlotView& location);
    virtual void onTileInstantiated();
    virtual void onTileCloned();

    virtual std::unique_ptr<TileController> clone() const;
};
