#pragma once

#include <vector>

#include <SFML/Graphics/Color.hpp>

class TileRarity
{
private:
    static const std::vector<sf::Color> m_colors;

    int m_rarity;

public:
    TileRarity(int rarity);

    sf::Color color() const;
    int rarity() const;
};