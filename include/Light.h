#pragma once

#include "../LibS/Shapes.h"
#include "../LibS/Noise.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class LightParams
{
public:
    LightParams() = default;
    LightParams(float radius, const sf::Color& color);

    float radius() const;
    const sf::Color& color() const;

private:
    float m_radius;
    sf::Color m_color;
};

class Light
{
public:
    Light() = default;
    Light(const LightParams& params, const ls::Vec2F& position, const void* owner);

    const LightParams& params() const;
    const ls::Vec2F& position() const;
    float radius() const;
    const sf::Color& color() const;
    const void* owner() const;

    ls::Rectangle2F bounds() const;

    static constexpr int maxLightRadius()
    {
        return m_maxLightRadius;
    }

private:
    static constexpr int m_maxLightRadius = 4;

    LightParams m_params;
    ls::Vec2F m_position;
    const void* m_owner;
};