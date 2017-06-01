#include "tiles/TileAttribute.h"


const std::map<std::string, TileAttributeId> TileAttributeIdHelper::m_dictionary{
    { "Attack", TileAttributeId::Attack },
    { "Defense", TileAttributeId::Defense },
};

TileAttributeId TileAttributeIdHelper::stringToEnum(const std::string& s)
{
    return m_dictionary.at(s);
}
std::string TileAttributeIdHelper::enumToString(TileAttributeId req)
{
    for (const auto& p : m_dictionary)
    {
        if (p.second == req) return p.first;
    }

    return "";
}