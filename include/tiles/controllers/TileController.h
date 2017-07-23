#pragma once

#include "Configuration.h"
#include "tiles/TileComponent.h"
#include "ComponentCommonData.h"
#include "DynamicEnum.h"

#include <memory>

#include "../LibS/Shapes.h"

class Tile;
class TileLocation;
class Player;
class World;
class InventorySlotView;

struct AmmoGroup;
using TileAmmoGroupType = RuntimeIdentifier<AmmoGroup>;

class TileController : public TileComponent<TileController, Tile>
	//must not be abstract (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    struct AttackResult
    {
        int weaponUsed;
        int ammoUsed;
    };

    TileController(Tile& owner);
    TileController(const TileController& other, Tile& owner);
    ~TileController() override;

    virtual void use(Player& player, const TileLocation& location);
    virtual void use(Player& player, const InventorySlotView& location);
    virtual void look(Player& player, const TileLocation& location);
    virtual void look(Player& player, const InventorySlotView& location);
    virtual AttackResult attack(World& world, Player& player, const ls::Vec2F& hintedPosition);
    virtual void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition);
    virtual TileAmmoGroupType ammoGroup() const;

    std::unique_ptr<TileController> clone(Tile& owner) const override;
};
