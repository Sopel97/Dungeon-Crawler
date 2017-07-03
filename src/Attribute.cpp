#include "Attribute.h"


const std::map<std::string, AttributeId> AttributeIdHelper::m_stringToEnum{
    { "Attack", AttributeId::Attack },
    { "Defense", AttributeId::Defense },
};
const std::vector<std::string> AttributeIdHelper::m_enumToString{
    "Attack",
    "Defense"
};

AttributeId AttributeIdHelper::stringToEnum(const std::string& s)
{
    return m_stringToEnum.at(s);
}
std::string AttributeIdHelper::enumToString(AttributeId id)
{
    return m_enumToString[static_cast<int>(id)];
}

std::string Attribute::formatToString() const
{
    const auto& traits = AttributeHelper::traits(id);

    std::string result = traits.displayedName;

    result.append(": ");

    if (value > 0 && traits.isSignForced) 
        result.append("+");

    result.append(std::to_string(value));

    if (traits.format == AttributeValueFormat::Percentage) 
        result.append("%");

    return result;
}

sf::Color Attribute::color() const
{
    return value > 0 ? AttributeHelper::traits(id).colorPositive : AttributeHelper::traits(id).colorNegative;
}

const std::vector<AttributeTraits> AttributeHelper::m_traits{
    {AttributeId::Attack, "Attack", AttributeValueFormat::Scalar, sf::Color::Green, sf::Color::Red, false},
    {AttributeId::Defense, "Defense", AttributeValueFormat::Scalar, sf::Color::Green, sf::Color::Red, false}
};
const AttributeTraits& AttributeHelper::traits(AttributeId id)
{
    return m_traits[static_cast<int>(id)];
}