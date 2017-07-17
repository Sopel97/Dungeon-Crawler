#pragma once

#include <vector>

#include <SFML/Graphics/Color.hpp>

struct TileInformationLine
{
    std::string text;
    sf::Color color;
};

class TileInformation
{
public:
    using Iterator = std::vector<TileInformationLine>::iterator;
    using ConstIterator = std::vector<TileInformationLine>::const_iterator;

    TileInformation() = default;
    TileInformation(std::vector<TileInformationLine>&& lines) :
        m_lines(std::move(lines))
    {

    }

    Iterator begin()
    {
        return m_lines.begin();
    }
    ConstIterator begin() const
    {
        return m_lines.begin();
    }
    Iterator end()
    {
        return m_lines.end();
    }
    ConstIterator end() const
    {
        return m_lines.end();
    }
    ConstIterator cbegin() const
    {
        return m_lines.begin();
    }
    ConstIterator cend() const
    {
        return m_lines.end();
    }


private:
    std::vector<TileInformationLine> m_lines;
};