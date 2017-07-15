#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

class SpriteBatch : public sf::Drawable
{
public:
    SpriteBatch();

    void emplaceRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& texCoords, const ls::Vec2F& size, const sf::Color& color = sf::Color::White);

    void clear();

private:
    static constexpr int m_defaultNumBuffers = 32;
    static constexpr int m_defaultBufferSize = 256;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

    std::vector<sf::Vertex>& createBuffer(const sf::Texture* texture);

    std::vector<sf::Vertex>& findBuffer(const sf::Texture* texture);

    std::unordered_map<const sf::Texture*, std::vector<sf::Vertex>> m_vertexBuffersByTexture;
};