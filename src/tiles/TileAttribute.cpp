#include "tiles/TileAttribute.h"


const std::map<std::string, TileAttributeId> TileAttributeIdHelper::m_stringToEnum{
    { "Attack", TileAttributeId::Attack },
    { "Defense", TileAttributeId::Defense },
};
const std::vector<std::string> TileAttributeIdHelper::m_enumToString{
    "Attack",
    "Defense"
};

TileAttributeId TileAttributeIdHelper::stringToEnum(const std::string& s)
{
    return m_stringToEnum.at(s);
}
std::string TileAttributeIdHelper::enumToString(TileAttributeId id)
{
    return m_enumToString[static_cast<int>(id)];
}

std::string TileAttribute::formatToString() const
{
    const auto& traits = TileAttributeHelper::traits(id);

    std::string result = traits.displayedName;

    result.append(": ");

    if (value > 0 && traits.isSignForced) 
        result.append("+");

    result.append(std::to_string(value));

    if (traits.format == TileAttributeValueFormat::Percentage) 
        result.append("%");

    return result;
}

sf::Color TileAttribute::color() const
{
    return value > 0 ? TileAttributeHelper::traits(id).colorPositive : TileAttributeHelper::traits(id).colorNegative;
}

const std::vector<TileAttributeTraits> TileAttributeHelper::m_traits{
    {TileAttributeId::Attack, "Attack", TileAttributeValueFormat::Scalar, sf::Color::Green, sf::Color::Red, false},
    {TileAttributeId::Defense, "Defense", TileAttributeValueFormat::Scalar, sf::Color::Green, sf::Color::Red, false}
};
const TileAttributeTraits& TileAttributeHelper::traits(TileAttributeId id)
{
    return m_traits[static_cast<int>(id)];
}