#pragma once

#include "Light.h"

#include "Configuration.h"

#include <optional>

class OscillatingLightSource
{
public:
    OscillatingLightSource(const LightParams& light1, const LightParams& light2, double frequency);

    static OscillatingLightSource fromConfig(ConfigurationNode& config);

    LightParams at(double t) const;
    LightParams at(double t, double seed) const;

    LightParams now() const;
    LightParams now(double seed) const;

    static double seedFromPointer(const void* ptr);

private:
    LightParams m_light1;
    LightParams m_light2;
    double m_frequency;
};

namespace ConfigurationLoaders
{
    inline void load(std::optional<OscillatingLightSource>& lightSource, ConfigurationNode& config)
    {
        if (!config.exists())
        {
            lightSource = std::nullopt;
            return;
        }

        lightSource = OscillatingLightSource::fromConfig(config);
    }
}