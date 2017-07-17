#pragma once

#include <variant>
#include <optional>

#include "../LibS/Shapes.h"

#include "TileLocation.h"

class Player;
class InventorySystem;
class Inventory;
class World;
class TileStack;
class TileColumn;

class TileTransferMediator
{
public:
    struct FromWorld
    {
        ls::Vec2I pos;
        World* world;
        Player* player;
    };
    struct FromInventory
    {
        InventorySystem* inventorySystem;
        Inventory* inventory;
        int slot;
    };
    struct ToWorld
    {
        ls::Vec2I pos;
        World* world;
        Player* player;
    };
    struct ToInventory
    {
        InventorySystem* inventorySystem;
        Inventory* inventory;
        int slot;
    };
private:

    std::optional<std::variant<FromWorld, FromInventory>> m_source;

public:

    void grabFromWorld(const ls::Vec2I& loc, World& world, Player& player);
    void grabFromInventory(InventorySystem& invSys, Inventory& inv, int slot);
    void putToWorld(const ls::Vec2I& loc, World& world, Player& player);
    void putToInventory(InventorySystem& invSys, Inventory& inv, int slot);
    void reset();

    bool isAnyTileGrabbed() const;

    void operator()(const FromWorld& from, const ToWorld& to);
    void operator()(const FromWorld& from, const ToInventory& to);
    void operator()(const FromInventory& from, const ToWorld& to);
    void operator()(const FromInventory& from, const ToInventory& to);
private:
    static std::vector<int> move(MapLayer& map, int fromX, int fromY, int toX, int toY, int max);
    static int move(MapLayer& map, int fromX, int fromY, Inventory& inventory, int toSlot, int max);
    static std::vector<int> move(MapLayer& map, int fromX, int fromY, Inventory& inventory, int max);
    static int move(Inventory& fromInventory, int fromSlot, Inventory& toInventory, int toSlot, int max);
    static std::vector<int> move(Inventory& fromInventory, int fromSlot, Inventory& toInventory, int max);
    static std::vector<int> move(Inventory& fromInventory, int fromSlot, MapLayer& map, int toX, int toY, int max);
};