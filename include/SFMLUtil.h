#ifndef SFMLUTIL_H
#define SFMLUTIL_H

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class SFMLUtil final
{
public:
    SFMLUtil() = delete;
    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& spritePosition, const ls::Vec2F& spriteSize, sf::Color color = sf::Color::White);
    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& spritePosition, const sf::Vector2f& spriteSize, sf::Color color = sf::Color::White);
    static void appendAsTriangleList(sf::VertexArray& vertexArray, const ls::RectangleF& rect, sf::Color color = sf::Color::White);
protected:
};

#endif // SFMLUTIL_H
