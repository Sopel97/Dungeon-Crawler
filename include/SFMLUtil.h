#ifndef SFMLUTIL_H
#define SFMLUTIL_H

#include "../LibS/GeometryLight.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class SFMLUtil final
{
public:
    SFMLUtil() = delete;
    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const Geo::Vec2F& position, const Geo::Vec2F& size, const Geo::Vec2F& spritePosition, const Geo::Vec2F& spriteSize, sf::Color color = sf::Color::White);
    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& spritePosition, const sf::Vector2f& spriteSize, sf::Color color = sf::Color::White);
    static void appendAsTriangleList(sf::VertexArray& vertexArray, const Geo::RectangleF& rect, sf::Color color = sf::Color::White);
protected:
};

#endif // SFMLUTIL_H
