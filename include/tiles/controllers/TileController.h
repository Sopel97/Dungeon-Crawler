#pragma once

#include "Configuration.h"
#include "tiles/TileComponent.h"
#include "ComponentCommonData.h"

#include <memory>

class Tile;
class TileLocation;
class Player;
class InventorySlotView;

class TileController : public TileComponent<TileController, Tile>
	//must not be abstract (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileController();
    TileController(const TileController& other);
    virtual ~TileController();

    virtual void use(Player& player, const TileLocation& location);
    virtual void use(Player& player, const InventorySlotView& location);
    virtual void look(Player& player, const TileLocation& location);
    virtual void look(Player& player, const InventorySlotView& location);

    virtual std::unique_ptr<TileController> clone() const;
};
