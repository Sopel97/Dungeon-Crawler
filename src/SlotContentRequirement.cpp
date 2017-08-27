#include "SlotContentRequirement.h"

const std::map<std::string, SlotContentRequirement> SlotContentRequirementHelper::m_dictionary {
    { "None", SlotContentRequirement::None },
    { "Helmet", SlotContentRequirement::Helmet },
    { "Chestplate", SlotContentRequirement::Chestplate },
    { "Pants", SlotContentRequirement::Pants },
    { "Boots", SlotContentRequirement::Boots },
    { "Weapon", SlotContentRequirement::Weapon },
    { "Shield", SlotContentRequirement::Shield },
    { "Necklace", SlotContentRequirement::Necklace },
    { "Ring", SlotContentRequirement::Ring },
    { "Ammo", SlotContentRequirement::Ammo },
    { "Container", SlotContentRequirement::Container },
    { "LightSource", SlotContentRequirement::LightSource }
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
ls::Vec2I SlotContentRequirementHelper::sprite(SlotContentRequirement req)
{
    static const std::map<SlotContentRequirement, ls::Vec2I> requirementIcons
    {
        { SlotContentRequirement::Necklace, ls::Vec2I(38 + 32 * 0, 19) },
        { SlotContentRequirement::Helmet, ls::Vec2I(38 + 32 * 1, 19) },
        { SlotContentRequirement::Container, ls::Vec2I(38 + 32 * 2, 19) },
        { SlotContentRequirement::Weapon, ls::Vec2I(38 + 32 * 3, 19) },
        { SlotContentRequirement::Shield, ls::Vec2I(38 + 32 * 4, 19) },

        { SlotContentRequirement::Chestplate, ls::Vec2I(38 + 32 * 0, 19 + 32) },
        { SlotContentRequirement::Pants, ls::Vec2I(38 + 32 * 1, 19 + 32) },
        { SlotContentRequirement::Ring, ls::Vec2I(38 + 32 * 2, 19 + 32) },
        { SlotContentRequirement::Ammo, ls::Vec2I(38 + 32 * 3, 19 + 32) },
        { SlotContentRequirement::Boots, ls::Vec2I(38 + 32 * 4, 19 + 32) },

        { SlotContentRequirement::LightSource, ls::Vec2I(38 + 32 * 0, 19 + 32 * 2) }
    };

    return requirementIcons.at(req);
}