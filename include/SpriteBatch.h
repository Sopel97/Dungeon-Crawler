#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

#include <array>

class SpriteBatch : public sf::Drawable
{
public:
    struct SpriteGeometry
    {
        std::array<sf::Vertex, 6> vertices;
    };

    SpriteBatch();

    void emplaceRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const sf::Color& color = sf::Color::White);
    void emplaceRotatedRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Vec2F& rotation, const ls::Vec2F& origin, const sf::Color& color = sf::Color::White);
    void emplaceRotatedRectangle(const sf::Texture* texture, const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Angle2F& rotation, const ls::Vec2F& origin, const sf::Color& color = sf::Color::White);
    void emplaceGeometry(const sf::Texture* texture, const SpriteGeometry& geometry);

    SpriteGeometry geometryFromRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const sf::Color& color = sf::Color::White);
    SpriteGeometry geometryFromRotatedRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Vec2F& rotation, const ls::Vec2F& origin, const sf::Color& color = sf::Color::White);
    SpriteGeometry geometryFromRotatedRectangle(const ls::Vec2F& position, const ls::Vec2F& size, const ls::Vec2F& texCoords, const ls::Vec2F& texSize, const ls::Angle2F& rotation, const ls::Vec2F& origin, const sf::Color& color = sf::Color::White);

    void clear();

    template <class Func>
    void apply(Func&& func)
    {
        for (auto& vertexArray : m_vertexBuffersByTexture)
        {
            for (auto& vert : vertexArray.second)
            {
                std::forward<Func>(func)(vert);
            }
        }
    }

private:
    static constexpr int m_defaultNumBuffers = 32;
    static constexpr int m_defaultBufferSize = 4092;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override;

    std::vector<sf::Vertex>& createBuffer(const sf::Texture* texture);

    std::vector<sf::Vertex>& findBuffer(const sf::Texture* texture);

    std::unordered_map<const sf::Texture*, std::vector<sf::Vertex>> m_vertexBuffersByTexture;
};