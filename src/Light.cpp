#include "Light.h"

#include <SFML/Graphics.hpp>

Light::Light(const ls::Vec2F& position, float radius, const sf::Color& color) :
    m_position(position),
    m_radius(radius),
    m_color(color)
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
const sf::Color& Light::color() const
{
    return m_color;
}
void Light::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const sf::Texture& texture) const
{
    const ls::Vec2F topLeft = m_position - ls::Vec2F(m_radius, m_radius);

    sf::RectangleShape playerLightShape;
    playerLightShape.setTexture(&texture, true);
    playerLightShape.setPosition(sf::Vector2f(topLeft.x, topLeft.y));
    playerLightShape.setSize(sf::Vector2f(m_radius * 2.0f, m_radius * 2.0f));
    playerLightShape.setFillColor(m_color);

    renderTarget.draw(playerLightShape, renderStates);
}