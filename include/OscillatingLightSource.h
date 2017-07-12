#pragma once

#include "Light.h"

class OscillatingLightSource
{
public:
    OscillatingLightSource(const Light& light1, const Light& light2, double frequency);

    Light at(double t) const;
    Light at(double t, double seed) const;
private:
    Light m_light1;
    Light m_light2;
    double m_frequency;
};