#pragma once

#include <memory>

#include "Tile.h"

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
    void insert(int delta);
    void erase(int delta);
    int spaceLeft() const;

    Tile& tile();
    const Tile& tile() const;

    TileStack split(int newStackSize);

    TileStack clone() const;
};

