#pragma once

#include "ConfigurationReader.h"

#include "Attribute.h"

namespace ConfigurationReaders
{
    struct Reader<AttributeId>
    {
        static void read(AttributeId& attributeId, ConfigurationNode& config)
        {
            attributeId = AttributeIdHelper::stringToEnum(config.get<std::string>());
        }
    };
}