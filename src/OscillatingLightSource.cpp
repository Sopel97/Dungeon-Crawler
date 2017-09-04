#include "OscillatingLightSource.h"

#include "GameTime.h"

OscillatingLightSource::OscillatingLightSource(const LightParams& light1, const LightParams& light2, double frequency) :
    m_light1(light1),
    m_light2(light2),
    m_frequency(frequency)
{

}

OscillatingLightSource OscillatingLightSource::fromConfig(ConfigurationNode& config)
{
    ConfigurationNode light1Config = config["light1"];
    const float rad1 = light1Config["radius"].get<float>();
    const int r1 = light1Config["color"][1].get<int>();
    const int g1 = light1Config["color"][2].get<int>();
    const int b1 = light1Config["color"][3].get<int>();

    ConfigurationNode light2Config = config["light2"];
    const float rad2 = light2Config["radius"].get<float>();
    const int r2 = light2Config["color"][1].get<int>();
    const int g2 = light2Config["color"][2].get<int>();
    const int b2 = light2Config["color"][3].get<int>();

    const float freq = config["freq"].get<float>();

    return OscillatingLightSource(
        LightParams(rad1, sf::Color(r1, g1, b1)),
        LightParams(rad2, sf::Color(r2, g2, b2)),
        freq
    );
}

LightParams OscillatingLightSource::at(double t) const
{
    constexpr double noiseScale = 1.5;

    ls::SimplexNoise1D noiseSampler;
    noiseSampler.setLowerBound(0.0);
    noiseSampler.setUpperBound(1.0);
    noiseSampler.setOctaves(3);
    noiseSampler.setScale(m_frequency);

    const double rad1 = m_light1.radius();
    const double r1 = m_light1.color().r;
    const double g1 = m_light1.color().g;
    const double b1 = m_light1.color().b;

    const double rad2 = m_light2.radius();
    const double r2 = m_light2.color().r;
    const double g2 = m_light2.color().g;
    const double b2 = m_light2.color().b;

    const double sample = std::clamp((noiseSampler(t) - 0.5)*noiseScale + 0.5, 0.0, 1.0);

    const double rad = rad1 + (rad2 - rad1)*sample;
    const double r = r1 + (r2 - r1)*sample;
    const double g = g1 + (g2 - g1)*sample;
    const double b = b1 + (b2 - b1)*sample;

    return LightParams(
        static_cast<float>(rad),
        sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b))
    );
}

LightParams OscillatingLightSource::at(double t, double seed) const
{
    constexpr double noiseScale = 1.5;

    ls::SimplexNoise2D noiseSampler;
    noiseSampler.setLowerBound(0.0);
    noiseSampler.setUpperBound(1.0);
    noiseSampler.setOctaves(3);
    noiseSampler.setScale(ls::Vec2D(m_frequency, m_frequency));

    const double rad1 = m_light1.radius();
    const double r1 = m_light1.color().r;
    const double g1 = m_light1.color().g;
    const double b1 = m_light1.color().b;

    const double rad2 = m_light2.radius();
    const double r2 = m_light2.color().r;
    const double g2 = m_light2.color().g;
    const double b2 = m_light2.color().b;

    const double sample = std::clamp((noiseSampler(ls::Vec2D(t, seed)) - 0.5)*noiseScale + 0.5, 0.0, 1.0);

    const double rad = rad1 + (rad2 - rad1)*sample;
    const double r = r1 + (r2 - r1)*sample;
    const double g = g1 + (g2 - g1)*sample;
    const double b = b1 + (b2 - b1)*sample;

    return LightParams(
        static_cast<float>(rad),
        sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b))
    );
}
LightParams OscillatingLightSource::now() const
{
    return at(GameTime::instance().now());
}
LightParams OscillatingLightSource::now(double seed) const
{
    return at(GameTime::instance().now(), seed);
}
double OscillatingLightSource::seedFromPointer(const void* ptr)
{
    constexpr uintptr_t period = uintptr_t(1) << uintptr_t(16);

    return static_cast<double>(reinterpret_cast<uintptr_t>(ptr) % period);
}