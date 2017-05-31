#include "TileDescription.h"

TileDescriptionLine::TileDescriptionLine(const std::string& text, const sf::Color& color, sf::Text::Style style, TileDescriptionFontSize fontSize) :
    m_text(text),
    m_color(color),
    m_style(style),
    m_fontSize(fontSize)
{

}

const std::string& TileDescriptionLine::text() const
{
    return m_text;
}
const sf::Color& TileDescriptionLine::color() const
{
    return m_color;
}
sf::Text::Style TileDescriptionLine::style() const
{
    return m_style;
}
TileDescriptionFontSize TileDescriptionLine::fontSize() const
{
    return m_fontSize;
}


TileDescription::TileDescription()
{

}

TileDescription::Iterator TileDescription::begin()
{
    return m_lines.begin();
}
TileDescription::ConstIterator TileDescription::begin() const
{
    return m_lines.begin();
}
TileDescription::ConstIterator TileDescription::cbegin() const
{
    return m_lines.cbegin();
}
TileDescription::Iterator TileDescription::end()
{
    return m_lines.end();
}
TileDescription::ConstIterator TileDescription::end() const
{
    return m_lines.end();
}
TileDescription::ConstIterator TileDescription::cend() const
{
    return m_lines.cend();
}

const TileDescriptionLine& TileDescription::line(int i) const
{
    return m_lines[i];
}

void TileDescription::emplaceLine(const std::string& text, const sf::Color& color, sf::Text::Style style, TileDescriptionFontSize fontSize)
{
    m_lines.emplace_back(text, color, style, fontSize);
}
void TileDescription::addLine(const TileDescriptionLine& line)
{
    m_lines.emplace_back(line);
}
void TileDescription::addLine(TileDescriptionLine&& line)
{
    m_lines.emplace_back(std::move(line));
}

int TileDescription::numLines() const
{
    return m_lines.size();
}
