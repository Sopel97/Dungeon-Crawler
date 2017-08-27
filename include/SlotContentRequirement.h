#pragma once

#include <map>
#include <string>

#include "../LibS/Shapes.h"

enum class SlotContentRequirement
{
    None,
    Helmet,
    Chestplate,
    Pants,
    Boots,
    Weapon,
    Shield,
    Necklace,
    Ring,
    Ammo,
    Container,
    LightSource
};

class SlotContentRequirementHelper
{
private:
    static const std::map<std::string, SlotContentRequirement> m_dictionary;
public:

    static SlotContentRequirement stringToEnum(const std::string& s);
    static std::string enumToString(SlotContentRequirement req);

    static ls::Vec2I sprite(SlotContentRequirement req);
};