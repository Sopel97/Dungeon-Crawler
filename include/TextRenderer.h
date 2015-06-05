#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <map>

#include "../LibS/GeometryLight.h"

class TextRenderer
{
public:
    enum class Alignment
    {
        Left,
        Right,
        Center
    };
    struct CharacterInformation
    {
        Geo::Vec2I characterSpritePosition;
        Geo::Vec2I backgroundSpritePosition;
        Geo::Vec2I characterSize;
    };

    static TextRenderer& instance();
    void drawFancyNumber(std::string numberString, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const Geo::Vec2F& position, Alignment align, const sf::Color& color = sf::Color::White) const;
    const CharacterInformation& characterInformation(char c) const;
    const Geo::Vec2I& fancyNumberCharacterSpritePosition(char c) const;
    const Geo::Vec2I& fancyNumberBackgroundSpritePosition(char c) const;
    const Geo::Vec2I& fancyNumberCharacterSize(char c) const; //full size
private:
    TextRenderer();
    void preprocessFancyNumberString(std::string& number) const;
    Vec2I calculateTopLeft(const std::string& text, const Vec2F& position, Alignment align) const;

    const int m_fancyNumberSpaceBetweenCharacters = -2;
    const int m_fancyNumberTopMargin = -3;
    std::map<char, CharacterInformation> m_fancyNumberCharset;
};
#endif // TEXTRENDERER_H
