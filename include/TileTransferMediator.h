#pragma once

#include <variant>
#include <optional>

#include "TileLocation.h"

class Player;
class InventorySystem;
class Inventory;

class TileTransferMediator
{
public:
    struct FromWorld
    {
        TileLocation location;
    };
    struct FromInventory
    {
        Inventory* inventory;
        int slot;
    };
    struct ToWorld
    {
        TileLocation location;
    };
    struct ToInventory
    {
        Inventory* inventory;
        int slot;
    };
private:
    InventorySystem* m_inventorySystem;
    Player* m_player;

    std::optional<std::variant<FromWorld, FromInventory>> m_source;

public:
    TileTransferMediator(InventorySystem& inventorySystem, Player& player);

    void grab(const FromWorld& from);
    void grab(const FromInventory& from);
    void put(const ToWorld& to);
    void put(const ToInventory& to);

    bool isAnyTileGrabbed() const;

    void operator()(const FromWorld& from, const ToWorld& to);
    void operator()(const FromWorld& from, const ToInventory& to);
    void operator()(const FromInventory& from, const ToWorld& to);
    void operator()(const FromInventory& from, const ToInventory& to);
private:
};