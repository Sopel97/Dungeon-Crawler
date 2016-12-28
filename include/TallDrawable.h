#ifndef TALLDRAWABLE_H
#define TALLDRAWABLE_H

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}

class TallDrawable
{
public:
    enum class DrawableType
    {
        TileStack,
        Entity
    };

    TallDrawable();
    virtual ~TallDrawable();

    virtual const ls::Rectangle2F& boundingRectangle() const = 0;
    virtual const ls::Vec2F& center() const = 0;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;

    virtual DrawableType type() const = 0;

    static bool compare(const TallDrawable& lhs, const TallDrawable& rhs);
    static bool ptrCompare(const TallDrawable* lhs, const TallDrawable* rhs);
protected:
};

#endif // TALLDRAWABLE_H
