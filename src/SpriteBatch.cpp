#include "SpriteBatch.h"


SpriteBatch::SpriteBatch() :
    m_vertexBuffersByTexture(m_defaultNumBuffers)
{

}

void SpriteBatch::emplaceRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& texCoords, const ls::Vec2F& size, const sf::Color& color)
{
    const sf::Vertex topLeft(sf::Vector2f(position.x, position.y), color, sf::Vector2f(texCoords.x, texCoords.y));
    const sf::Vertex topRight(sf::Vector2f(position.x + size.x, position.y), color, sf::Vector2f(texCoords.x + size.x, texCoords.y));
    const sf::Vertex bottomLeft(sf::Vector2f(position.x, position.y + size.y), color, sf::Vector2f(texCoords.x, texCoords.y + size.y));
    const sf::Vertex bottomRight(sf::Vector2f(position.x + size.x, position.y + size.y), color, sf::Vector2f(texCoords.x + size.x, texCoords.y + size.y));

    auto& buffer = findBuffer(texture);

    buffer.emplace_back(topLeft);
    buffer.emplace_back(topRight);
    buffer.emplace_back(bottomRight);

    buffer.emplace_back(topLeft);
    buffer.emplace_back(bottomRight);
    buffer.emplace_back(bottomLeft);
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