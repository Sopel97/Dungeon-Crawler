#pragma once

#include <memory>

#include "Tile.h"

class TileStack
{
private:
    std::unique_ptr<Tile> m_tile;
    int m_quantity;

public:
    TileStack(std::unique_ptr<Tile>&& tile, int quantity);
    ~TileStack() = default;

    int quantity() const;
    void setQuantity(int newQuantity);
    int maxQuantity() const;

    Tile& tile();
    const Tile& tile() const;

    std::unique_ptr<TileStack> clone() const;
};

