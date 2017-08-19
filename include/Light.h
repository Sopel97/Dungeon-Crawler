#pragma once

#include "../LibS/Shapes.h"
#include "../LibS/Noise.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Light
{
public:
    Light(const ls::Vec2F& position, float radius, const sf::Color& color);

    const ls::Vec2F& position() const;
    float radius() const;
    const sf::Color& color() const;

    const ls::Rectangle2F bounds() const;

    void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const sf::Texture& texture) const;

private:
    ls::Vec2F m_position;
    float m_radius;
    sf::Color m_color;
};