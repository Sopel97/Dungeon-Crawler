#include "SpriteBatch.h"

#include "../LibS/Geometry.h"

SpriteBatch::SpriteBatch() :
    m_vertexBuffersByTexture(m_defaultNumBuffers)
{

}

void SpriteBatch::emplaceRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const sf::Color& color)
{
    emplaceGeometry(texture, geometryFromRectangle(position, size, texCoords, texSize, color));
}
void SpriteBatch::emplaceRotatedRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Vec2F& rotation, const ls::Vec2F& origin, const sf::Color& color)
{
    emplaceRotatedRectangle(texture, position, size, texCoords, texSize, rotation.angle(), origin, color);
}
void SpriteBatch::emplaceRotatedRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Angle2F& rotation, const ls::Vec2F& origin, const sf::Color& color)
{
    emplaceGeometry(texture, geometryFromRotatedRectangle(position, size, texCoords, texSize, rotation, origin, color));
}
void SpriteBatch::emplaceGeometry(const sf::Texture* texture, const SpriteGeometry& geometry)
{
    auto& buffer = findBuffer(texture);
    buffer.insert(buffer.end(), geometry.vertices.begin(), geometry.vertices.end());
}

SpriteBatch::SpriteGeometry SpriteBatch::geometryFromRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const sf::Color& color)
{
    const sf::Vertex topLeft(sf::Vector2f(position.x, position.y), color, sf::Vector2f(texCoords.x, texCoords.y));
    const sf::Vertex topRight(sf::Vector2f(position.x + size.x, position.y), color, sf::Vector2f(texCoords.x + texSize.x, texCoords.y));
    const sf::Vertex bottomLeft(sf::Vector2f(position.x, position.y + size.y), color, sf::Vector2f(texCoords.x, texCoords.y + texSize.y));
    const sf::Vertex bottomRight(sf::Vector2f(position.x + size.x, position.y + size.y), color, sf::Vector2f(texCoords.x + texSize.x, texCoords.y + texSize.y));

    return SpriteGeometry{ topLeft, topRight, bottomRight, topLeft, bottomRight, bottomLeft };
}
SpriteBatch::SpriteGeometry SpriteBatch::geometryFromRotatedRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Vec2F& rotation, const ls::Vec2F& origin, const sf::Color& color)
{
    return geometryFromRotatedRectangle(position, size, texCoords, texSize, rotation.angle(), origin, color);
}
SpriteBatch::SpriteGeometry SpriteBatch::geometryFromRotatedRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Angle2F& rotation, const ls::Vec2F& origin, const sf::Color& color)
{
    const ls::Vec2F topLeft(position.x, position.y);
    const ls::Vec2F topRight(position.x + size.x, position.y);
    const ls::Vec2F bottomLeft(position.x, position.y + size.y);
    const ls::Vec2F bottomRight(position.x + size.x, position.y + size.y);

    sf::Transform transform;
    transform.translate(origin.x, origin.y).rotate(rotation.degrees() + 90).translate(-origin.x, -origin.y);

    const auto p0 = transform.transformPoint(topLeft.x, topLeft.y);
    const auto p1 = transform.transformPoint(topRight.x, topRight.y);
    const auto p2 = transform.transformPoint(bottomLeft.x, bottomLeft.y);
    const auto p3 = transform.transformPoint(bottomRight.x, bottomRight.y);

    const sf::Vertex v0(sf::Vector2f(p0.x, p0.y), color, sf::Vector2f(texCoords.x, texCoords.y));
    const sf::Vertex v1(sf::Vector2f(p1.x, p1.y), color, sf::Vector2f(texCoords.x + texSize.x, texCoords.y));
    const sf::Vertex v2(sf::Vector2f(p2.x, p2.y), color, sf::Vector2f(texCoords.x, texCoords.y + texSize.y));
    const sf::Vertex v3(sf::Vector2f(p3.x, p3.y), color, sf::Vector2f(texCoords.x + texSize.x, texCoords.y + texSize.y));

    return SpriteGeometry{ v0, v1, v3, v0, v3, v2 };
}

void SpriteBatch::clear()
{
    m_vertexBuffersByTexture.clear();
}
void SpriteBatch::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{
    for (auto& p : m_vertexBuffersByTexture)
    {
        auto& buffer = p.second;

        if (buffer.size() == 0) continue;

        renderStates.texture = p.first;
        renderTarget.draw(buffer.data(), buffer.size(), sf::PrimitiveType::Triangles, renderStates);
    }
}
std::vector<sf::Vertex>& SpriteBatch::createBuffer(const sf::Texture* texture)
{
    auto iter = m_vertexBuffersByTexture.emplace(texture, std::vector<sf::Vertex>{});

    std::vector<sf::Vertex>& buffer = iter.first->second;
    buffer.reserve(m_defaultBufferSize);
    return buffer;
}

std::vector<sf::Vertex>& SpriteBatch::findBuffer(const sf::Texture* texture)
{
    auto iter = m_vertexBuffersByTexture.find(texture);
    if (iter == m_vertexBuffersByTexture.end()) return createBuffer(texture);

    return iter->second;
}