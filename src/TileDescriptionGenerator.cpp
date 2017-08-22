#include "TileDescriptionGenerator.h"

#include "TileDescription.h"

#include "AttributeSet.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

#include "tiles/TileInformation.h"

#include "Inventory.h"

TileDescription TileDescriptionGenerator::generate(const Tile& tile, int quantity)
{
    TileDescription desc;
    
    std::string prefix = tile.model().prefix();
    if (!prefix.empty()) prefix.append(" ");
    std::string fullName = prefix + tile.model().displayedName();
    if (quantity > 1) fullName += " x" + std::to_string(quantity);
    desc.emplaceLine(fullName, tile.model().rarity().color(), sf::Text::Style::Regular, 26);

    TileInformation information = tile.model().additionalInformation();
    for (const auto& line : information)
    {
        desc.emplaceLine(line.text, line.color, sf::Text::Style::Regular, 16);
    }

    const AttributeSet& attributes = tile.model().attributes();
    for (const auto& attribute : attributes)
    {
        desc.emplaceLine(attribute.formatToString(), attribute.color(), sf::Text::Style::Regular, 16);
    }

    return desc;
}
