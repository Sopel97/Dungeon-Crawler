#include "sprite/Spritesheet.h"

Spritesheet::Spritesheet(const std::string& path, int gridSize, int padding) :
    m_texture{},
    m_gridSize(gridSize),
    m_padding(padding)
{
    m_texture.loadFromFile(path);
}

ls::Vec2I Spritesheet::gridCoordsToTexCoords(const ls::Vec2I& gridCoords) const
{
    return gridCoords * (m_gridSize + m_padding);
}
ls::Vec2I Spritesheet::gridSizeToTexSize(const ls::Vec2I& onGridSize) const
{
    return onGridSize * m_gridSize;
}

const sf::Texture& Spritesheet::texture()
{
    return m_texture;
}
int Spritesheet::gridSize() const
{
    return m_gridSize;
}
int Spritesheet::padding() const
{
    return m_padding;
}

void Spritesheet::setRepeated(bool repeated)
{
    m_texture.setRepeated(repeated);
}