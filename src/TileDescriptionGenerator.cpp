#include "TileDescriptionGenerator.h"

#include "TileDescription.h"

#include "AttributeSet.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

#include "Inventory.h"

TileDescription TileDescriptionGenerator::generate(const Tile& tile)
{
    TileDescription desc;
    
    desc.emplaceLine(tile.model().displayedName(), sf::Color::Red, sf::Text::Style::Regular, 26);

    if (const Inventory* inv = tile.model().inventory())
    {
        const Inventory& inventory = *inv;
        const int inventorySize = inventory.size();
        desc.emplaceLine(std::string("Capacity: ") + std::to_string(inventorySize), sf::Color::Green, sf::Text::Style::Regular, 16);
    }

    const AttributeSet& attributes = tile.model().attributes();
    for (const auto& attribute : attributes)
    {
        desc.emplaceLine(attribute.formatToString(), attribute.color(), sf::Text::Style::Regular, 16);
    }

    return desc;
}
