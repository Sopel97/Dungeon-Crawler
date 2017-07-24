#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Configuration.h"

#include "../LibS/Shapes.h"

class Spritesheet
{
public:
    Spritesheet(const std::string& path, int gridSize, int padding);

    ls::Vec2I gridCoordsToTexCoords(const ls::Vec2I& gridCoords) const;
    ls::Vec2I gridSizeToTexSize(const ls::Vec2I& onGridSize) const;

    const sf::Texture& texture();
    int gridSize() const;
    int padding() const;

    void setRepeated(bool repeated = true);

private:
    sf::Texture m_texture;
    int m_gridSize;
    int m_padding;
};