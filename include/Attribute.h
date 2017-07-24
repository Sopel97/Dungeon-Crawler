#pragma once

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>

enum class AttributeId
{
    Attack,
    Defense,
    NumAttributes
};


class AttributeIdHelper
{
private:
    static const std::map<std::string, AttributeId> m_stringToEnum;
    static const std::vector<std::string> m_enumToString;

public:
    static AttributeId stringToEnum(const std::string& s);
    static std::string enumToString(AttributeId id);

    static constexpr int numAttributes()
    {
        return static_cast<int>(AttributeId::NumAttributes);
    }
};

enum class AttributeValueFormat
{
    Scalar,
    Percentage
};

struct AttributeTraits
{
    AttributeId id;
    std::string displayedName;
    AttributeValueFormat format;
    sf::Color colorPositive;
    sf::Color colorNegative;
    bool isSignForced;
};

struct Attribute
{
    using ValueType = int;

    AttributeId id;
    ValueType value;

    bool operator==(const Attribute& other) const;
    bool operator!=(const Attribute& other) const;

    std::string formatToString() const;
    sf::Color color() const;
};

class AttributeHelper
{
private:
    static const std::vector<AttributeTraits> m_traits;
public:
    static const AttributeTraits& traits(AttributeId id);
};