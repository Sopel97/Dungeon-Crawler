#include "SFMLUtil.h"

using namespace ls;

void SFMLUtil::appendQuadAsTriangleList(sf::VertexArray& vertexArray, const Vec2F& position, const Vec2F& size, const Vec2F& spritePosition, const Vec2F& spriteSize, sf::Color color)
{
    appendQuadAsTriangleList(vertexArray, sf::Vector2f(position.x, position.y), sf::Vector2f(size.x, size.y), sf::Vector2f(spritePosition.x, spritePosition.y), sf::Vector2f(spriteSize.x, spriteSize.y), color);
}

void SFMLUtil::appendQuadAsTriangleList(sf::VertexArray& vertexArray, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& spritePosition, const sf::Vector2f& spriteSize, sf::Color color)
{
    vertexArray.append(sf::Vertex(position, color, spritePosition));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color, spritePosition + sf::Vector2f(0.0f, spriteSize.y)));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(spriteSize.x, spriteSize.y)));

    vertexArray.append(sf::Vertex(position, color, spritePosition));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(spriteSize.x, spriteSize.y)));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color, spritePosition + sf::Vector2f(spriteSize.x, 0.0f)));
}

void SFMLUtil::appendAsTriangleList(sf::VertexArray& vertexArray, const ls::Rectangle2F& rect, sf::Color color)
{
    appendQuadAsTriangleList(vertexArray, rect.min, Vec2F(rect.width(), rect.height()), Vec2F(0.0f, 0.0f), Vec2F(0.0f, 0.0f), color);
}
