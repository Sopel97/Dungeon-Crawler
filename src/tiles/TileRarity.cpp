#include "tiles/TileRarity.h"

const std::vector<sf::Color> TileRarity::m_colors{
    sf::Color(128, 128, 128), // junk
    sf::Color(255, 255, 255), // common
    sf::Color(0, 128, 16), // uncommon
    sf::Color(0, 200, 255), // kinda rare
    sf::Color(0, 40, 255), // rare
    sf::Color(128, 0, 255), // very rare
    sf::Color(255, 0, 255), // super rare
    sf::Color(255, 220, 0), // legendary
    sf::Color(255, 140, 0), // super legendary
    sf::Color(255, 0, 0) // otherwordly
};

TileRarity::TileRarity() :
    m_rarity(0)
{

}
TileRarity::TileRarity(int rarity) :
    m_rarity(rarity)
{

}

sf::Color TileRarity::color() const
{
    if (m_rarity >= m_colors.size()) return sf::Color::Black;

    return m_colors[m_rarity];
}
int TileRarity::rarity() const
{
    return m_rarity;
}