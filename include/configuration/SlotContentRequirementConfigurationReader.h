#pragma once

#include "ConfigurationReader.h"

#include "SlotContentRequirement.h"

namespace ConfigurationReaders
{
    template<>
    struct Reader<SlotContentRequirement>
    {
        static void read(SlotContentRequirement& slot, ConfigurationNode& config)
        {
            slot = SlotContentRequirementHelper::stringToEnum(config.get<std::string>());
        }
    };
}