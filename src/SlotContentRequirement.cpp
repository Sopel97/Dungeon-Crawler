#include "SlotContentRequirement.h"

const std::map<std::string, SlotContentRequirement> SlotContentRequirementHelper::m_dictionary {
    { "None", SlotContentRequirement::None },
    { "Helmet", SlotContentRequirement::Helmet },
    { "Chestplate", SlotContentRequirement::Chestplate },
    { "Pants", SlotContentRequirement::Pants },
    { "Boots", SlotContentRequirement::Boots },
    { "RightHand", SlotContentRequirement::RightHand },
    { "LeftHand", SlotContentRequirement::LeftHand },
    { "Necklace", SlotContentRequirement::Necklace },
    { "Ring", SlotContentRequirement::Ring },
    { "Ammo", SlotContentRequirement::Ammo },
    { "Container", SlotContentRequirement::Container }
};

SlotContentRequirement SlotContentRequirementHelper::stringToEnum(const std::string& s)
{
    return m_dictionary.at(s);
}
std::string SlotContentRequirementHelper::enumToString(SlotContentRequirement req)
{
    for (const auto& p : m_dictionary)
    {
        if (p.second == req) return p.first;
    }

    return "";
}