#include "Light.h"

#include <SFML/Graphics.hpp>

Light::Light(const ls::Vec2F& position, float radius, const sf::Color& firstColor) :
    m_position(position),
    m_radius(radius),
    m_firstColor(firstColor),
    m_secondColor(firstColor),
    m_frequency(0.0)
{

}
Light::Light(const ls::Vec2F& position, float radius, const sf::Color& firstColor, const sf::Color& secondColor, double frequency) :
    m_position(position),
    m_radius(radius),
    m_firstColor(firstColor),
    m_secondColor(secondColor),
    m_frequency(frequency)
{

}

const ls::Vec2F& Light::position() const
{
    return m_position;
}
float Light::radius() const
{
    return m_radius;
}
const sf::Color& Light::firstColor() const
{
    return m_firstColor;
}
const sf::Color& Light::secondColor() const
{
    return m_secondColor;
}
sf::Color Light::color(double t) const
{
    ls::SimplexNoise1D noiseSampler;
    noiseSampler.setLowerBound(0.0);
    noiseSampler.setUpperBound(1.0);
    noiseSampler.setOctaves(2);
    noiseSampler.setScale(m_frequency);

    const float r1 = m_firstColor.r;
    const float g1 = m_firstColor.g;
    const float b1 = m_firstColor.b;
    const float r2 = m_secondColor.r;
    const float g2 = m_secondColor.g;
    const float b2 = m_secondColor.b;

    const double sample = noiseSampler(t);

    const float r = r1 + (r2 - r1)*sample;
    const float g = g1 + (g2 - g1)*sample;
    const float b = b1 + (b2 - b1)*sample;

    return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b));
}
void Light::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const sf::Texture& texture, double t) const
{
    const ls::Vec2F topLeft = m_position - ls::Vec2F(m_radius, m_radius);

    sf::RectangleShape playerLightShape;
    playerLightShape.setTexture(&texture, true);
    playerLightShape.setPosition(sf::Vector2f(topLeft.x, topLeft.y));
    playerLightShape.setSize(sf::Vector2f(m_radius * 2.0f, m_radius * 2.0f));
    playerLightShape.setFillColor(color(t));

    renderTarget.draw(playerLightShape, renderStates);
}