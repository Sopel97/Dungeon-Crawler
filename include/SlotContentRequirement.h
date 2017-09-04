#pragma once

#include "Configuration.h"

#include <map>
#include <string>
#include <set>

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

using SlotContentRequirementSet = std::set<SlotContentRequirement>;

namespace ConfigurationLoaders
{
    inline void load(SlotContentRequirementSet& slots, ConfigurationNode& config)
    {
        if (config.exists())
        {
            const int numEntries = config.length();
            for (int i = 1; i <= numEntries; ++i)
            {
                slots.insert(SlotContentRequirementHelper::stringToEnum(config[i].get<std::string>()));
            }
        }
    }
}
