#pragma once

#include <memory>

#include "Tile.h"
#include "TileUseResult.h"
#include "TileAttackResult.h"

#include "../LibS/Shapes.h"

class TileStack
{
private:
    std::unique_ptr<Tile> m_tile;
    int m_quantity;

public:
    TileStack();

    TileStack(std::unique_ptr<Tile>&& tile, int quantity);

    TileStack(const TileStack&) = delete; //could be done but it's better not to
    TileStack(TileStack&&) = default;
    TileStack& operator=(const TileStack&) = delete;
    TileStack& operator=(TileStack&&) = default;

    ~TileStack() = default;

    bool isEmpty() const;

    int quantity() const;
    int maxQuantity() const;

    void setQuantity(int newQuantity);
    void addTiles(int delta);
    void removeTiles(int delta);
    int spaceLeft() const;

    TileUseResult use(Player& player, const TileLocation& location);
    TileUseResult use(Player& player, const InventorySlotLocation& location);
    void look(Player& player, const TileLocation& location);
    void look(Player& player, const InventorySlotLocation& location);
    TileAttackResult attack(World& world, Player& player, const ls::Vec2F& hintedPosition);
    void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition);

    Tile& tile();
    const Tile& tile() const;

    TileStack split(int newStackSize);

    TileStack clone() const;
};

