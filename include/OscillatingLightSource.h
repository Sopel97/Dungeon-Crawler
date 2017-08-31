#pragma once

#include "Light.h"

#include "Configuration.h"

class OscillatingLightSource
{
public:
    OscillatingLightSource(const LightParams& light1, const LightParams& light2, double frequency);

    static OscillatingLightSource fromConfig(ConfigurationNode& config);

    LightParams at(double t) const;
    LightParams at(double t, double seed) const;

private:
    LightParams m_light1;
    LightParams m_light2;
    double m_frequency;
};