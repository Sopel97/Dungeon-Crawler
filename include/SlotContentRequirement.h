#pragma once

#include <map>

enum class SlotContentRequirement
{
    None,
    Helmet,
    Chestplate,
    Pants,
    Boots,
    RightHand,
    LeftHand,
    Necklace,
    Ring,
    Ammo,
    Container
};

class SlotContentRequirementHelper
{
private:
    static const std::map<std::string, SlotContentRequirement> m_dictionary;
public:

    static SlotContentRequirement stringToEnum(const std::string& s);
    static std::string enumToString(SlotContentRequirement req);
};