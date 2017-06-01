#pragma once

#include <map>
#include <string>

enum class TileAttributeId
{
    Attack,
    Defense
};

class TileAttributeIdHelper
{
private:
    static const std::map<std::string, TileAttributeId> m_dictionary;
public:

    static TileAttributeId stringToEnum(const std::string& s);
    static std::string enumToString(TileAttributeId req);
};

struct TileAttribute
{
    TileAttributeId id;
    int value;
};