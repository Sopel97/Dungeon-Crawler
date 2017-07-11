#pragma once

#include "../LibS/Shapes.h"
#include "../LibS/Noise.h"

#include <SFML/Graphics/Color.hpp>

class Light
{
public:
    Light(const ls::Vec2F& position, float radius, const sf::Color& firstColor);
    Light(const ls::Vec2F& position, float radius, const sf::Color& firstColor, const sf::Color& secondColor, double frequency);

    const ls::Vec2F& position() const;
    float radius() const;
    const sf::Color& firstColor() const;
    const sf::Color& secondColor() const;
    sf::Color color(double t) const;

private:
    ls::Vec2F m_position;
    float m_radius;
    sf::Color m_firstColor;
    sf::Color m_secondColor;
    double m_frequency;
};