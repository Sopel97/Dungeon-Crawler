#pragma once

#include "ConfigurationReader.h"

#include "OscillatingLightSource.h"

namespace ConfigurationReaders
{
    template<>
    struct Reader<OscillatingLightSource>
    {
        static void read(OscillatingLightSource& lightSource, ConfigurationNode& config)
        {
            lightSource = OscillatingLightSource::fromConfig(config);
        }
    };
}
