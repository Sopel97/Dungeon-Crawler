#ifndef TALLDRAWABLE_H
#define TALLDRAWABLE_H

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}

class TallDrawable
{
public:
    TallDrawable();
    virtual ~TallDrawable();

    virtual bool isTile() const;
    virtual bool isEntity() const;

    virtual const Geo::RectangleF& boundingRectangle() const = 0;
    virtual const Geo::Vec2F& center() const = 0;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;

    static bool compare(const TallDrawable& lhs, const TallDrawable& rhs);
protected:
};

#endif // TALLDRAWABLE_H
