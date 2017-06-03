#include "TileDescriptionGenerator.h"

#include "TileDescription.h"

#include "tiles/Tile.h"
#include "tiles/TileAttributeSet.h"
#include "tiles/models/TileModel.h"

TileDescription TileDescriptionGenerator::generate(const Tile& tile)
{
    TileDescription desc;
    
    desc.emplaceLine(tile.model().displayedName(), sf::Color::Red, sf::Text::Style::Bold, 20);

    const TileAttributeSet& attributes = tile.model().attributes();
    for (const auto& attribute : attributes)
    {
        desc.emplaceLine(TileAttributeIdHelper::enumToString(attribute.id) + std::string(": ") + std::to_string(attribute.value), sf::Color::Green);
    }

    return desc;
}
