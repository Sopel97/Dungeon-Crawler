#include "Light.h"

#include <SFML/Graphics.hpp>

LightParams::LightParams(float radius, const sf::Color& color) :
    m_radius(radius),
    m_color(color)
{

}

float LightParams::radius() const
{
    return m_radius;
}
const sf::Color& LightParams::color() const
{
    return m_color;
}

Light::Light(const LightParams& params, const ls::Vec2F& position, const void* owner) :
    m_params(params),
    m_position(position),
    m_owner(owner)
{

}

const ls::Vec2F& Light::position() const
{
    return m_position;
}
float Light::radius() const
{
    return m_params.radius();
}
const sf::Color& Light::color() const
{
    return m_params.color();
}
const void* Light::owner() const
{
    return m_owner;
}

ls::Rectangle2F Light::bounds() const
{
    const ls::Vec2F radiusVector(radius(), radius());
    return ls::Rectangle2F(m_position - radiusVector, m_position + radiusVector);
}
