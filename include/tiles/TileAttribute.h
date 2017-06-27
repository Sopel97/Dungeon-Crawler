#pragma once

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>

enum class TileAttributeId
{
    Attack,
    Defense,
    NumAttributes
};


class TileAttributeIdHelper
{
private:
    static const std::map<std::string, TileAttributeId> m_stringToEnum;
    static const std::vector<std::string> m_enumToString;

public:
    static TileAttributeId stringToEnum(const std::string& s);
    static std::string enumToString(TileAttributeId id);

    static constexpr int numAttributes()
    {
        return static_cast<int>(TileAttributeId::NumAttributes);
    }
};

enum class TileAttributeValueFormat
{
    Scalar,
    Percentage
};

struct TileAttributeTraits
{
    TileAttributeId id;
    std::string displayedName;
    TileAttributeValueFormat format;
    sf::Color colorPositive;
    sf::Color colorNegative;
    bool isSignForced;
};

struct TileAttribute
{
    TileAttributeId id;
    int value;

    std::string formatToString() const;
    sf::Color color() const;
};

class TileAttributeHelper
{
private:
    static const std::vector<TileAttributeTraits> m_traits;
public:
    static const TileAttributeTraits& traits(TileAttributeId id);
};