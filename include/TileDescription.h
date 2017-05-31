#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

enum class TileDescriptionFontSize
{
    Small,
    Normal,
    Large
};

class TileDescriptionLine
{
private:
    std::string m_text;
    sf::Color m_color;
    sf::Text::Style m_style;
    TileDescriptionFontSize m_fontSize;

public:
    TileDescriptionLine(const std::string& text, const sf::Color& color, sf::Text::Style style = sf::Text::Style::Regular, TileDescriptionFontSize fontSize = TileDescriptionFontSize::Normal);

    const std::string& text() const;
    const sf::Color& color() const;
    sf::Text::Style style() const;
    TileDescriptionFontSize fontSize() const;

};

class TileDescription
{
public:
    using LineStorageType = std::vector<TileDescriptionLine>;
    using Iterator = typename LineStorageType::iterator;
    using ConstIterator = typename LineStorageType::const_iterator;
private:
    std::vector<TileDescriptionLine> m_lines;

public:
    TileDescription();

    Iterator begin();
    ConstIterator begin() const;
    ConstIterator cbegin() const;
    Iterator end();
    ConstIterator end() const;
    ConstIterator cend() const;

    const TileDescriptionLine& line(int i) const;

    void emplaceLine(const std::string& text, const sf::Color& color, sf::Text::Style style = sf::Text::Style::Regular, TileDescriptionFontSize fontSize = TileDescriptionFontSize::Normal);
    void addLine(const TileDescriptionLine& line);
    void addLine(TileDescriptionLine&& line);

    int numLines() const;
};