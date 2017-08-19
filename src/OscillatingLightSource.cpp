#include "OscillatingLightSource.h"

OscillatingLightSource::OscillatingLightSource(const Light& light1, const Light& light2, double frequency) :
    m_light1(light1),
    m_light2(light2),
    m_frequency(frequency)
{

}

Light OscillatingLightSource::at(double t) const
{
    ls::SimplexNoise1D noiseSampler;
    noiseSampler.setLowerBound(0.0);
    noiseSampler.setUpperBound(1.0);
    noiseSampler.setOctaves(3);
    noiseSampler.setScale(m_frequency);

    const double x1 = m_light1.position().x;
    const double y1 = m_light1.position().y;
    const double rad1 = m_light1.radius();
    const double r1 = m_light1.color().r;
    const double g1 = m_light1.color().g;
    const double b1 = m_light1.color().b;

    const double x2 = m_light2.position().x;
    const double y2 = m_light2.position().y;
    const double rad2 = m_light2.radius();
    const double r2 = m_light2.color().r;
    const double g2 = m_light2.color().g;
    const double b2 = m_light2.color().b;

    const double sample = noiseSampler(t);

    const double x = x1 + (x2 - x1)*sample;
    const double y = y1 + (y2 - y1)*sample;
    const double rad = rad1 + (rad2 - rad1)*sample;
    const double r = r1 + (r2 - r1)*sample;
    const double g = g1 + (g2 - g1)*sample;
    const double b = b1 + (b2 - b1)*sample;

    return Light(
        ls::Vec2F(x, y),
        rad,
        sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b))
    );
}

Light OscillatingLightSource::at(double t, double seed) const
{
    ls::SimplexNoise2D noiseSampler;
    noiseSampler.setLowerBound(0.0);
    noiseSampler.setUpperBound(1.0);
    noiseSampler.setOctaves(3);
    noiseSampler.setScale(ls::Vec2D(m_frequency, m_frequency));

    const float x1 = m_light1.position().x;
    const float y1 = m_light1.position().y;
    const float rad1 = m_light1.radius();
    const float r1 = m_light1.color().r;
    const float g1 = m_light1.color().g;
    const float b1 = m_light1.color().b;

    const float x2 = m_light2.position().x;
    const float y2 = m_light2.position().y;
    const float rad2 = m_light2.radius();
    const float r2 = m_light2.color().r;
    const float g2 = m_light2.color().g;
    const float b2 = m_light2.color().b;

    const double sample = noiseSampler(ls::Vec2D(t, seed));

    const float x = x1 + (x2 - x1)*sample;
    const float y = y1 + (y2 - y1)*sample;
    const float rad = rad1 + (rad2 - rad1)*sample;
    const float r = r1 + (r2 - r1)*sample;
    const float g = g1 + (g2 - g1)*sample;
    const float b = b1 + (b2 - b1)*sample;

    return Light(
        ls::Vec2F(x, y),
        rad,
        sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b))
    );
}