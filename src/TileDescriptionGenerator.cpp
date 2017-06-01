#include "TileDescriptionGenerator.h"

#include "TileDescription.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"

TileDescription TileDescriptionGenerator::generate(const Tile& tile)
{
    TileDescription desc;
    
    desc.emplaceLine(tile.model().displayedName(), sf::Color::Red, sf::Text::Style::Bold, 20);
    desc.emplaceLine("asdasdasdsa", sf::Color::Green);
    desc.emplaceLine("21312", sf::Color::Blue);
    desc.emplaceLine("asdasdasdsa", sf::Color::Yellow);
    desc.emplaceLine("123123213213211", sf::Color::Magenta);
    desc.emplaceLine("italics lul", sf::Color(128, 128, 128), sf::Text::Style::Italic, 10);

    return desc;
}
